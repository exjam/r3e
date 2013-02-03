#include "Global.h"

#include "Ray.hpp"
#include "Map.hpp"
#include "SkyBox.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Project.hpp"
#include "ROSEData.hpp"
#include "ShaderPair.hpp"
#include "SceneManager.hpp"
#include "ObjectShaderData.hpp"
#include "TerrainShaderData.hpp"

SceneManager::SceneManager()
	: mCamera(0), mCulling(true), mHoverEntity(0), mMap(0), mSkyBox(0), mSizeChanged(false)
{
	mShaders.SetSize(MAX_ENTITY);
}

SceneManager::~SceneManager(){
	ClearScene();
	mShaders.DeleteValues();
}

void SceneManager::SetMap(Map* map){
	SAFE_DELETE(mMap);
	mMap = map;
	mMap->AddObjectsToScene(this);
}

void SceneManager::SetSkyBox(SkyBox* box){
	SAFE_DELETE(mSkyBox);
	mSkyBox = box;
}

void SceneManager::ClearScene(){
	SAFE_DELETE(mMap);
	SAFE_DELETE(mSkyBox);
	mHoverEntity = NULL;
	for(unsigned int i = 0; i < MAX_ENTITY; ++i)
		mEntityList[i].Clear();

	for(unsigned int i = 0; i < ENTITY_FLAG_COUNT; ++i)
		mEntityFlag[i].Clear();

	mAllEntities.DeleteValues();
}

Entity* SceneManager::GetHoverEntity(){
	return mHoverEntity;
}

Entity* SceneManager::AddEntity(Entity* entity){
	if(!entity) return NULL;

	mEntityList[entity->mType].PushBack(entity);
	mAllEntities.PushBack(entity);

	if(entity->mFlags != ENTITY_NO_FLAGS){
		for(unsigned int i = 0; i < ENTITY_FLAG_COUNT; ++i){
			if(!((entity->mFlags >> i) & 1)) continue;
			mEntityFlag[i].PushBack(entity);
		}
	}

	return entity;
}

Entity* SceneManager::RemoveEntity(Entity* entity){
	if(!entity) return NULL;
	if(mHoverEntity == entity) mHoverEntity = NULL;

	mEntityList[entity->mType].Erase(mEntityList[entity->mType].Find(entity));
	mAllEntities.Erase(mAllEntities.Find(entity));

	if(entity->mFlags != ENTITY_NO_FLAGS){
		for(unsigned int i = 0; i < ENTITY_FLAG_COUNT; ++i){
			if(!((entity->mFlags >> i) & 1)) continue;
			mEntityFlag[i].Erase(mEntityFlag[i].Find(entity));
		}
	}

	return entity;
}

void SceneManager::SetCamera(Camera* camera){
	mCamera = camera;
}

void SceneManager::SetCulling(bool enabled){
	mCulling = enabled;
}

void SceneManager::Init(){
	ShaderPair* terrainShader = new ShaderPair("R3EShaders\\terrain.vsh", "R3EShaders\\terrain.psh");
	ShaderPair* objectShader  = new ShaderPair("R3EShaders\\2tex_object.vsh", "R3EShaders\\2tex_object.psh");
	ShaderPair* tex1Shader    = new ShaderPair("R3EShaders\\1tex_mesh.vsh", "R3EShaders\\1tex_mesh.psh");

	ObjectShaderData::LoadFromShader(objectShader->GetProgramObject());
	TerrainShaderData::LoadFromShader(terrainShader->GetProgramObject());

	mShaders[ENTITY_2TEX_OBJECT]  = objectShader;
	mShaders[ENTITY_3TEX_TERRAIN] = terrainShader;
	mShaders[ENTITY_1TEX_MESH]    = tex1Shader;
}

void SceneManager::MouseMove(int x, int y, int /*flags*/){
	Vector2 wnd = Vector2(float(mWidth), float(mHeight));
	Vector3 mouse = Vector3(float(x), float(mHeight - y), 0.0f);

	mHoverEntity = 0;

	if(!mEntityFlag[ENTITY_CAN_HOVER_IDX].Size()) return;
	Array<Entity*, 10>& lst = mEntityFlag[ENTITY_CAN_HOVER_IDX];

	Ray ray = UnprojectRay(mouse, mCamera->GetTransform(), mCamera->GetProjection(), wnd);
	Vector3 camPos = mCamera->GetTransform().TransformCoord(Vector3(0.0f, 0.0f, 0.0f));
	float nearest = 9999.0f;
	float dist;
	for(unsigned int i = 0; i < lst.Size(); ++i){
		Entity* entity = lst[i];
		if(!entity->mVisible) continue;
		if(!ray.IntersectsBox(entity->GetBoundingBox(), dist)) continue;
		if(dist > nearest) continue;
		nearest = dist;
		mHoverEntity = entity;
	}
}

void SceneManager::BeginScene(){
	if(mSizeChanged)
		UpdateViewport();

	OpenGL::mVertexCount = 0;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE_ARB);
}

void SceneManager::UpdateScene(){
	OpenGL::BindBuffer(GL_ARRAY_BUFFER, 0);

	if(mCamera && mCulling){
		bool viewChanged = mCamera->Apply();
		const Frustum& camf = mCamera->GetFrustum();
		if(viewChanged){
			if(mMap)
				mMap->UpdateCulling(camf);

			if(mSkyBox)
				mSkyBox->SetTransform(Matrix4::CreateTranslation(mCamera->GetCameraPosition()));
		}

		for(unsigned int i = 0; i < mEntityFlag[ENTITY_CAN_MOVE_IDX].Size(); ++i)
			mEntityFlag[ENTITY_CAN_MOVE_IDX].At(i)->UpdateCulling(camf);
	}

	for(unsigned int i = 0; i < mEntityFlag[ENTITY_CAN_UPDATE_IDX].Size(); ++i)
		mEntityFlag[ENTITY_CAN_UPDATE_IDX].At(i)->Update();
}

void SceneManager::EndScene(){
	glDisable(GL_MULTISAMPLE_ARB);
}

void SceneManager::UpdateViewport(){
	if(!mCamera) return;

	mCamera->SetViewport(mWidth, mHeight);
	mCamera->ApplyProjection();
}

void SceneManager::ResizeScene(int width, int height){
	mWidth = width;
	mHeight = height;

	mSizeChanged = true;
}

void SceneManager::Begin2D(){
	OpenGL::DepthTest(false);
	OpenGL::Blend(true);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0f, float(mWidth), float(mHeight), 0.0f, 1.0f, 1050.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void SceneManager::End2D(){
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	OpenGL::DepthTest(true);
}

void SceneManager::RenderScene(){
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if(mSkyBox) mSkyBox->Render();

	if(mEntityList[ENTITY_1TEX_MESH].Size()){
		Array<Entity*, 10>& lst = mEntityList[ENTITY_1TEX_MESH];

		mShaders[ENTITY_1TEX_MESH]->Apply();

		for(unsigned int i = 0; i < lst.Size(); ++i){
			if(!lst[i]->mVisible) continue;
#ifdef HIGHLIGHT_HOVER_ENTITY
			if(lst[i] == mHoverEntity) glColor3f(1.0f, 0.0f, 0.0f);
#endif
			lst[i]->Render();
#ifdef HIGHLIGHT_HOVER_ENTITY
			if(lst[i] == mHoverEntity) glColor3f(1.0f, 1.0f, 1.0f);
#endif
		}
	}

	OpenGL::ClientActiveTexture(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);

	if(mEntityList[ENTITY_2TEX_OBJECT].Size()){
		Array<Entity*, 10>& lst = mEntityList[ENTITY_2TEX_OBJECT];

		mShaders[ENTITY_2TEX_OBJECT]->Apply();

		OpenGL::Uniform1i(ObjectShaderData::mLayer1, 0);
		OpenGL::Uniform1i(ObjectShaderData::mLightmap, 1);

		for(unsigned int i = 0; i < lst.Size(); ++i){
			if(!lst[i]->mVisible) continue;
			lst[i]->Render();
		}

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	}

	if(mMap && mMap->mVisible){
		OpenGL::AlphaTest(false);
		OpenGL::CullFace(true);
		OpenGL::Blend(false);

		mShaders[ENTITY_3TEX_TERRAIN]->Apply();

		OpenGL::Uniform1i(TerrainShaderData::mLightmap, 0);
		OpenGL::Uniform1i(TerrainShaderData::mLayer1, 1);
		OpenGL::Uniform1i(TerrainShaderData::mLayer2, 2);

		OpenGL::ClientActiveTexture(GL_TEXTURE2_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);

		mMap->Render();

		OpenGL::ClientActiveTexture(GL_TEXTURE2_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	ShaderPair::Remove();

	OpenGL::ClientActiveTexture(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	OpenGL::BindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int SceneManager::EntityCount() const {
	return mAllEntities.Size();
}

Vector3 SceneManager::GetScreenPosition(Vector3& world) const {
	Vector3 pos;
	if(!mCamera) return pos;
	Point view(mWidth, mHeight);
	Project(world, mCamera->GetTransform(), mCamera->GetProjection(), view, pos);
	return pos;
}

Entity* SceneManager::CastRay(const Ray& ray, Vector3& out, float maxLengthSq) const {
	float closestDist = 9999999.0f;
	Vector3 closestPos;
	Vector3 tmpPos;
	Entity* nearEntity = NULL;
	bool hasHit = false;

	const Array<Entity*, 10>& lst = mEntityFlag[ENTITY_CAN_RAYMESH_IDX];
	for(unsigned int i = 0; i < lst.Size(); ++i){
		Entity* entity = lst[i];
		if(!entity->mVisible) continue;
		if(entity->CastRay(ray, tmpPos, maxLengthSq)){
			float dist = tmpPos.Distancesq(ray.mPoint);
			if(dist < closestDist){
				closestDist = dist;
				closestPos = tmpPos;
				hasHit = true;
				nearEntity = entity;
			}
		}
	}

	if(mMap && mMap->CastRay(ray, tmpPos)){
		float dist = tmpPos.Distancesq(ray.mPoint);
		if(dist < closestDist && dist > 0.0f){
			closestDist = dist;
			closestPos = tmpPos;
			hasHit = true;
			nearEntity = mMap;
		}
	}

	if(hasHit){
		out = closestPos;
		return nearEntity;
	}


	return NULL;
}

Entity* SceneManager::CastRayDown(const Ray& ray, Vector3& out) const {
	float closestDist = 9999999.0f;
	Vector3 closestPos;
	Vector3 tmpPos;
	Entity* nearEntity = NULL;
	bool hasHit = false;

	const Array<Entity*, 10>& lst = mEntityFlag[ENTITY_CAN_RAYMESH_IDX];
	for(unsigned int i = 0; i < lst.Size(); ++i){
		Entity* entity = lst[i];
		if(!entity->mVisible) continue;
		if(entity->CastRayDown(ray, tmpPos)){
			float dist = tmpPos.Distancesq(ray.mPoint);
			if(dist < closestDist){
				closestDist = dist;
				closestPos = tmpPos;
				hasHit = true;
				nearEntity = entity;
			}
		}
	}

	if(mMap && mMap->CastRayDown(ray, tmpPos)){
		float dist = tmpPos.Distancesq(ray.mPoint);
		if(dist < closestDist && dist > 0.0f){
			closestDist = dist;
			closestPos = tmpPos;
			hasHit = true;
			nearEntity = mMap;
		}
	}

	if(hasHit){
		out = closestPos;
		return nearEntity;
	}


	return NULL;
}