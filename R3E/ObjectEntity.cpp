#include "Global.h"

#include "OpenGL.hpp"
#include "EntityGroup.hpp"
#include "ObjectEntity.hpp"
#include "ObjectShaderData.hpp"

ObjectEntity::ObjectEntity(int objtype, int objid)
	: mObjectType(objtype), mObjectID(objid), mMeshData(0), mLitObject(0)
{
	mType = ENTITY_2TEX_OBJECT;
	mFlags = ENTITY_CAN_RAYMESH;
}

ObjectEntity::~ObjectEntity(){
	SAFE_DELETE(mMeshData);
}


void ObjectEntity::UpdateBoundingBox(){
	if(!mMeshData) return;

	mBoundingBox.Reset();
	mBoundingBoxTransformed.Reset();

	for(unsigned int i = 0; i < mMeshData->mChildren.Size(); ++i){
		mBoundingBox.AddBox(mMeshData->mChildren[i]->mBoundingBoxTransformed);
		mBoundingBoxTransformed.AddTransformedBox(mMeshData->mChildren[i]->mBoundingBoxTransformed, mTransform);
	}
}

void ObjectEntity::Render(){
	if(!mMeshData || !mLitObject) return;

	glPushMatrix();
	mTransform.glMult();

	for(unsigned int i = 0; i < mLitObject->mParts.Size(); ++i){
		ROSE::LIT::Object::Part* litpart = &mLitObject->mParts[i];
		if(litpart->mIndex >= mMeshData->mChildren.Size()) continue;

		OpenGL::ActiveTexture(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, litpart->mTexture->GetTextureID());

		OpenGL::ActiveTexture(GL_TEXTURE0_ARB);
		OpenGL::Uniform3f(ObjectShaderData::mLightmapTrans, litpart->xPos, litpart->yPos, litpart->scale);
		mMeshData->mChildren[litpart->mIndex]->Render();
	}

	glPopMatrix();
}

bool ObjectEntity::CastRay(const Ray& ray, Vector3& out, float maxLengthSq){
	if(!ray.IntersectsBox(mBoundingBoxTransformed)) return false;
	Vector3 center = (mBoundingBoxTransformed.mMax - mBoundingBoxTransformed.mMin) / 2;
	center += mBoundingBoxTransformed.mMin;
	if(center.Distance2Dsq(ray.mPoint) > maxLengthSq) return false;

	Ray localRay;
	Matrix4 invs = Matrix4::CreateInverse(mTransform);
	localRay.mPoint = invs.TransformCoord(ray.mPoint);
	localRay.mDirection = invs.TransformNormal(ray.mDirection);

	float closestDist = 9999999.0f;
	Vector3 closestPos;
	Vector3 tmpPos;
	bool hasHit = false;

	for(unsigned int i = 0; i < mLitObject->mParts.Size(); ++i){
		ROSE::LIT::Object::Part* litpart = &mLitObject->mParts[i];
		if(litpart->mIndex >= mMeshData->mChildren.Size()) continue;

		if(mMeshData->mChildren[litpart->mIndex]->CastRay(localRay, tmpPos, maxLengthSq)){
			float dist = tmpPos.Distancesq(localRay.mPoint);
			if(dist < closestDist){
				closestDist = dist;
				closestPos = tmpPos;
				hasHit = true;
			}
		}
	}

	if(hasHit)
		out = mTransform.TransformCoord(closestPos);

	return hasHit;
}

bool ObjectEntity::CastRayDown(const Ray& ray, Vector3& out){
	if(!mBoundingBoxTransformed.Contains2D(ray.mPoint)) return false;

	Ray localRay;
	Matrix4 invs = Matrix4::CreateInverse(mTransform);
	localRay.mPoint = invs.TransformCoord(ray.mPoint);
	localRay.mDirection = invs.TransformNormal(ray.mDirection);

	float closestDist = 9999999.0f;
	Vector3 closestPos;
	Vector3 tmpPos;
	bool hasHit = false;

	for(unsigned int i = 0; i < mLitObject->mParts.Size(); ++i){
		ROSE::LIT::Object::Part* litpart = &mLitObject->mParts[i];
		if(litpart->mIndex >= mMeshData->mChildren.Size()) continue;

		if(mMeshData->mChildren[litpart->mIndex]->CastRayDown(localRay, tmpPos)){
			float dist = tmpPos.Distancesq(localRay.mPoint);
			if(dist < closestDist){
				closestDist = dist;
				closestPos = tmpPos;
				hasHit = true;
			}
		}
	}

	if(hasHit)
		out = mTransform.TransformCoord(closestPos);

	return hasHit;
}

bool ObjectEntity::IsSkinned(){
	return false;
}

void ObjectEntity::SetLitObject(ROSE::LIT::Object* obj){
	mLitObject = obj;
}

int ObjectEntity::GetObjectID() const {
	return mObjectID;
}

int ObjectEntity::GetObjectType() const {
	return mObjectType;
}

void ObjectEntity::SetMesh(EntityGroup* mesh){
	if(!mesh) return;
	mMeshData = mesh;
	UpdateBoundingBox();
}
