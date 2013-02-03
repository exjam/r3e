#include "Global.h"

#include "EntityGroup.hpp"

EntityGroup::EntityGroup(){}
EntityGroup::~EntityGroup(){
	mChildren.DeleteValues();
}

void EntityGroup::UpdateAnimation(Matrix4* mat){
	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mChildren[i]->UpdateAnimation(mat);
}

int EntityGroup::UpdateCulling(const Frustum& camf){
	int ret = mBoundingBoxTransformed.CheckIntersectFrustum(camf);
	if(ret == 2){
		for(unsigned int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->mVisible = true;

		mVisible = true;
		return 2;
	}else if(ret == 0){
		for(unsigned int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->mVisible = false;

		mVisible = false;
		return 0;
	}

	mVisible = true;
	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mChildren[i]->UpdateCulling(camf);

	return 1;
}

void EntityGroup::Render(){
	if(mHasTransform){
		glPushMatrix();
		mTransform.glMult();
	}

	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mChildren[i]->Render();

	if(mHasTransform)
		glPopMatrix();
}

void EntityGroup::AddChild(Entity* child){
	mChildren.PushBack(child);
	mType = child->mType;
	UpdateBoundingBox();
}

Entity* EntityGroup::RemoveChild(Entity* child){
	mChildren.Erase(mChildren.Find(child));
	UpdateBoundingBox();
	return child;
}

void EntityGroup::SetGroupType(int type){
	mType = type;
}

void EntityGroup::UpdateBoundingBox(){
	mBoundingBox.Reset();
	mBoundingBoxTransformed.Reset();

	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mBoundingBox.AddBox(mChildren[i]->mBoundingBoxTransformed);

	mBoundingBoxTransformed.AddTransformedBox(mBoundingBox, mTransform);
}

bool EntityGroup::IsGroup(){
	return true;
}
