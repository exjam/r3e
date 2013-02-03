#include "Global.h"

#include "RoseData.hpp"
#include "ItemManager.hpp"
#include "AvatarEntity.hpp"
#include "MotionManager.hpp"

AvatarEntity::AvatarEntity(){
	mType = ENTITY_1TEX_MESH;
	mFlags = ENTITY_CAN_HOVER | ENTITY_CAN_MOVE | ENTITY_CAN_UPDATE;
	mAvatarState = AVT_ANI_STOP1;
	mItemEntities.SetSize(IT_MAX);
	mGender = 0;
	mHairStyle = 0;
}

AvatarEntity::~AvatarEntity(){
	//mItemEntities.DeleteValues();
}

void AvatarEntity::SetGender(int gender){
	if(gender != G_FEMALE && gender != G_MALE) return;

	mGender = gender;

	SetSkeleton(ROSE::Data::mAvatarSkeletons[gender]);
	UpdateAnimationType();
	UpdateHair();

	for(unsigned int i = 0; i < IT_MAX; ++i){
		if(!mItemEntities[i]) continue;
		SetItem(i, mItemEntities[i]->mID);
	}
}

bool AvatarEntity::SetItem(int type, int id){
	if(type >= IT_MAX) return false;
	if(type <= 0) return false;

	if(mItemEntities[type]){
		RemoveChild(mItemEntities[type]);
		delete mItemEntities[type];
		mItemEntities[type] = 0;
	}
	
	Entity* entity = ItemManager::Instance().LoadItem(id, type, mGender, this);
	if(!entity) return false;

	AddChild(entity);
	mItemEntities[type] = entity;

	if(type == IT_WEAPON) UpdateAnimationType();
	if(type == IT_CAP) UpdateHair();

	return true;
}

int AvatarEntity::GetAvatarState(){
	return mAvatarState;
}

void AvatarEntity::SetAvatarState(int state){
	mAvatarState = state;
	UpdateAnimationType();
}

void AvatarEntity::SetHairStyle(int style){
	mHairStyle = style;
	UpdateHair();
}

void AvatarEntity::SetFaceStyle(int style){
	SetItem(IT_FACE, style);
}

void AvatarEntity::UpdateAnimationType(){
	int weapon = 0;
	if(mItemEntities[IT_WEAPON]) weapon = mItemEntities[IT_WEAPON]->mID;
	SmartPointer<ROSE::ZMO> anim = MotionManager::Instance().LoadMotion(weapon, mAvatarState, mGender);
	SetAnimation(anim);
}

void AvatarEntity::UpdateHair(){
	ROSE::STB* hairSTB = ItemManager::Instance().GetSTB(IT_CAP);
	int cap = 0;
	if(mItemEntities[IT_CAP]) cap = mItemEntities[IT_CAP]->mID;
	int offset = hairSTB->GetInt(cap, 33);
	SetItem(IT_HAIR, (mHairStyle*5) + offset);
}
