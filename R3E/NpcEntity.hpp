#ifndef NPC_ENTITY_HPP
#define NPC_ENTITY_HPP

#include "NpcManager.hpp"
#include "RoseConstants.hpp"
#include "SkeletalEntity.hpp"

class NpcEntity : public SkeletalEntity {
public:
	NpcEntity(){
		mType = ENTITY_1TEX_MESH;
		mFlags = ENTITY_CAN_HOVER | ENTITY_CAN_MOVE | ENTITY_CAN_UPDATE;
		mState = MOB_ANI_STOP;
		mNpcID = 0;
	}

	virtual ~NpcEntity(){}

	bool SetNpcID(int id){
		mNpcID = id;
		if(!NpcManager::Instance().LoadNpcModel(this)) return false;
		return NpcManager::Instance().LoadNpcAnimation(mState, this);
	}

	bool SetState(int state){
		mState = state;
		return NpcManager::Instance().LoadNpcAnimation(mState, this);
	}

	int GetState(){
		return mState;
	}

	int NpcID(){
		return mNpcID;
	}

protected:
	int mState;
	int mNpcID;
};

#endif