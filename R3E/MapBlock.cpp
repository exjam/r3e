#include "Global.h"

#include "MapBlock.hpp"

#include "HIM.hpp"
#include "TIL.hpp"
#include "ZON.hpp"
#include "LIT.hpp"
#include "IFO.hpp"
#include "VertexTypes.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

MapBlock::MapBlock(IIndexBuffer* iBuffer)
	: mHeightmap(0), mBuildingLit(0), mObjectLit(0), mMapInfo(0), mTilemap(0), mVertexBuffer(0), mIndexBuffer(iBuffer), mQuadTree(4)
{
	mType = ENTITY_3TEX_TERRAIN;
}

MapBlock::~MapBlock(){
	SAFE_DELETE(mMapInfo);
	SAFE_DELETE(mTilemap);
	SAFE_DELETE(mHeightmap);
	SAFE_DELETE(mObjectLit);
	SAFE_DELETE(mBuildingLit);

	SAFE_DELETE(mVertexBuffer);
}

int MapBlock::UpdateCulling(const Frustum& camf){
	mQuadTree.UpdateCulling(camf, mPatchVisible);
	mVisible = mQuadTree.RootVisible();

	return 2;
}

void MapBlock::Render(){
	glPushMatrix();
	mTransform.glMult();
	glBindTexture(GL_TEXTURE_2D, mLightmap->GetTextureID());

	mVertexBuffer->BindBufferOnly();
	for(unsigned int i = 0; i < 16 * 16; ++i){
		if(!mPatchVisible[i]) continue;
		char* data = (char*)(i * (25*sizeof(V3F_UV0_UV1_UV2)));

		OpenGL::ActiveTexture(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, mLayer1[i]);

		OpenGL::ActiveTexture(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_2D, mLayer2[i]);

		glVertexPointer(3, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), data);

		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), data + sizeof(V3F));

		OpenGL::ClientActiveTexture(GL_TEXTURE1_ARB);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), data + sizeof(V3F_UV0));

		OpenGL::ClientActiveTexture(GL_TEXTURE2_ARB);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), data + sizeof(V3F_UV0_UV1));
		
		mIndexBuffer->Draw();

		OpenGL::mVertexCount += 25;
		OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);
	}

	glPopMatrix();

	OpenGL::ActiveTexture(GL_TEXTURE0_ARB);
}

void MapBlock::Update(){
}

void MapBlock::SetObjectLit(const char* path){
	SAFE_DELETE(mObjectLit);
	mObjectLit = new ROSE::LIT();
	mObjectLit->Load(path);
}

void MapBlock::SetBuildingLit(const char* path){
	SAFE_DELETE(mBuildingLit);
	mBuildingLit = new ROSE::LIT();
	mBuildingLit->Load(path);
}

void MapBlock::SetHeightmap(const char* path){
	SAFE_DELETE(mHeightmap);
	mHeightmap = new ROSE::HIM();
	mHeightmap->Load(path);
}

void MapBlock::SetTilemap(const char* path){
	SAFE_DELETE(mTilemap);
	mTilemap = new ROSE::TIL();
	mTilemap->Load(path);
}

void MapBlock::SetMapInfo(const char* path){
	SAFE_DELETE(mMapInfo);
	mMapInfo = new ROSE::IFO();
	mMapInfo->Load(path);
}

void MapBlock::SetLightmap(const char* path){
	mLightmap = TEX_MGR().Load(path);
}

void MapBlock::AddObjectsToScene(class SceneManager* scene){
	mMapInfo->AddObjectsToScene(scene);
	mMapInfo->LinkLightmaps(mObjectLit, mBuildingLit);
}

void MapBlock::GenerateTerrain(ROSE::ZON* zon){
	BoundingBox tmp;

	Range<float> blockHeight(9999.0f, -9999.0f);

	mVertexBuffer = new VertexBuffer<V3F_UV0_UV1_UV2>();
	mVertexBuffer->SetSize(16*16*5*5);
	for(int ix = 0; ix < 16; ++ix){
		for(int iy = 0; iy < 16; ++iy){
			int idx = iy + ix*16;
			Range<float> patchHeight(9999.0f, -9999.0f);

			ROSE::ZON::Tile* tile = zon->GetTile(mTilemap->GetTile(ix, iy));
			mLayer1[idx] = zon->GetTexture(tile->mLayer1 + tile->mOffset1)->GetTextureID();
			mLayer2[idx] = zon->GetTexture(tile->mLayer2 + tile->mOffset2)->GetTextureID();

			for(int jy = 0; jy < 5; ++jy){
				for(int jx = 0; jx < 5; ++jx){
					int dx4 = ix * 4 + jx;
					int dy4 = iy * 4 + jy;

					V3F_UV0_UV1_UV2 vert;
					vert.pos.x = dx4 * 2.5f;
					vert.pos.y = -(dy4 * 2.5f);
					vert.pos.z = mHeightmap->GetHeight(dx4, dy4);

					vert.uv0.x = float(dx4) / 64.0f;
					vert.uv0.y = float(dy4) / 64.0f;
					vert.uv1.x = vert.uv2.x = float(jx) / 4.0f;
					vert.uv1.y = vert.uv2.y = float(jy) / 4.0f;

					tile->RotateUV(vert.uv2.x, vert.uv2.y);

					mVertexBuffer->AddVertex(&vert);

					blockHeight.AddValue(vert.pos.z);
					patchHeight.AddValue(vert.pos.z);
				}
			}

			
			BoundingBox bbox;
			bbox.mMin = Vector3(float(ix * 4) * 2.5f, -(float((iy * 4) + 4) * 2.5f), patchHeight.mMin);
			bbox.mMax = Vector3((float(ix * 4) + 4) * 2.5f, -(float(iy * 4) * 2.5f), patchHeight.mMax);

			BoundingBox bboxTransformed;
			bboxTransformed.AddTransformedBox(bbox, mTransform);

			mQuadTree.SetIdxBox(idx, bboxTransformed);
		}
	}

	mQuadTree.Calculate();

	tmp.AddPoint(Vector3(0.0f, -160.0f, blockHeight.mMin));
	tmp.AddPoint(Vector3(160.0f, 0.0f, blockHeight.mMax));
	SetBoundingBox(tmp);
}

float MapBlock::GetHeight(float x, float y) const {
	Vector3 mpos = mTransform.GetTranslate();
	if(x < mpos.x) return 0.0f;
	if(y > mpos.y) return 0.0f;
	if(x > mpos.x + 160.0f) return 0.0f;
	if(y < mpos.y - 160.0f) return 0.0f;

	mpos.x = x - mpos.x;
	mpos.y = mpos.y - y;

	int ix = int(mpos.x / 2.5f) % 64;
	int iy = int(mpos.y / 2.5f) % 64;

	float tX = mpos.x - float(ix)*2.5f;
	float tY = mpos.y - float(iy)*2.5f;

	Vector3 rPoint(tX, tY, 100.0f);
	Vector3 v1(0.0f, 0.0f, mHeightmap->GetHeight(ix,  iy));
	Vector3 v2(2.5f, 0.0f, mHeightmap->GetHeight(ix+1,iy));
	Vector3 v3(0.0f, 2.5f, mHeightmap->GetHeight(ix,  iy+1));

	float t;
	Ray ray(rPoint, Vector3(0.0f, 0.0f, -1.0f));
	if(!ray.IntersectTriangle(v1, v2, v3, t)){
		Vector3 v4(2.5f, 2.5f, mHeightmap->GetHeight(ix+1,iy+1));
		if(!ray.IntersectTriangle(v2, v4, v3, t)) return 0.0f;
	}

	rPoint.z -= t;
	rPoint += mTransform.GetTranslate();
	return rPoint.z;
}

bool MapBlock::CastRayDown(const Ray& ray, float& tMin) const {
	Vector3 mpos = mTransform.GetTranslate();
	mpos.x = ray.mPoint.x - mpos.x;
	mpos.y = mpos.y - ray.mPoint.y;

	int ix = int(mpos.x / 2.5f) % 64;
	int iy = int(mpos.y / 2.5f) % 64;

	float tX = mpos.x - float(ix)*2.5f;
	float tY = mpos.y - float(iy)*2.5f;

	Vector3 v1(0.0f, 0.0f, mHeightmap->GetHeight(ix,  iy));
	Vector3 v2(2.5f, 0.0f, mHeightmap->GetHeight(ix+1,iy));
	Vector3 v3(0.0f, 2.5f, mHeightmap->GetHeight(ix,  iy+1));

	Vector3 rPoint(tX, tY, ray.mPoint.z);
	Ray nRay = Ray(rPoint, ray.mDirection);
	if(!nRay.IntersectTriangle(v1, v2, v3, tMin)){
		Vector3 v4(2.5f, 2.5f, mHeightmap->GetHeight(ix+1,iy+1));
		if(!nRay.IntersectTriangle(v2, v4, v3, tMin)) return false;
	}

	return true;
}

void MapBlock::CastRay(const Ray& ray, float& tMin) const {
	if(!mVisible) return;
	QuadIntersectRay(&mQuadTree.mRootNode, ray, tMin);
}

void MapBlock::QuadIntersectRay(const QuadTree::Node* node, const Ray& ray, float& tMin) const {
	if(!node->IsVisible()) return;
	if(!ray.IntersectsBox(node->mBoundingBox)) return;
	if(!node->mChildren[0]){
		GetPatchIntersect(node->mIndex, ray, tMin);
		return;
	}

	QuadIntersectRay(node->mChildren[0], ray, tMin);
	QuadIntersectRay(node->mChildren[1], ray, tMin);
	QuadIntersectRay(node->mChildren[2], ray, tMin);
	QuadIntersectRay(node->mChildren[3], ray, tMin);

	return;
}

bool MapBlock::GetPatchIntersect(int index, const Ray& ray, float& tMin) const {
	int ox = (index / 16) * 4;
	int oy = (index % 16) * 4;

	Vector3 root = mTransform.GetTranslate() + Vector3(2.5f * ox, -2.5f * oy, 0.0f);
	Vector3 trans = root;
	float t;
	bool ret = false;
	for(unsigned int x = 0; x < 5; ++x){
		for(unsigned int y = 0; y < 5; ++y){
			int ix = ox + x;
			int iy = oy + y;
			Vector3 v1(trans.x, trans.y, trans.z + mHeightmap->GetHeight(ix,  iy));
			Vector3 v2(trans.x + 2.5f, trans.y, trans.z + mHeightmap->GetHeight(ix+1,iy));
			Vector3 v3(trans.x, trans.y - 2.5f, trans.z + mHeightmap->GetHeight(ix,  iy+1));

			if(!ray.IntersectTriangle(v1, v3, v2, t)){
				Vector3 v4(trans.x + 2.5f, trans.y - 2.5f, trans.z + mHeightmap->GetHeight(ix+1,iy+1));
				if(!ray.IntersectTriangle(v2, v3, v4, t)){
					trans.y -= 2.5f;
					continue;
				}
			}

			if(t < tMin)
				tMin = t;

			ret = true;
		}

		trans.x += 2.5f;
		trans.y = root.y;
	}
	
	return ret;
}
