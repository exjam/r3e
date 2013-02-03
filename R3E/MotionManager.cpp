#include "Global.h"

#include "MotionManager.hpp"
#include "RoseConstants.hpp"

MotionManager MotionManager::mInstance;

MotionManager::MotionManager(){}
MotionManager::~MotionManager(){}

void MotionManager::Load(){
	mTypeMotion.Load("3DDATA\\STB\\TYPE_MOTION.STB");
	mFileMotion.Load("3DDATA\\STB\\FILE_MOTION.STB");
}

void MotionManager::SetWeaponSTB(ROSE::STB* stb){
	mWeaponSTB = stb;
}

SmartPointer<ROSE::ZMO> MotionManager::LoadMotion(int weapon, int action, int gender) const {
	if(action >= MAX_AVT_ANI) return SmartPointer<ROSE::ZMO>();
	int type = mWeaponSTB->GetInt(weapon, 34);
	int motion = mTypeMotion.GetInt(action, type);
	const char* file = mFileMotion.GetString(motion, gender);
	if(strlen(file) < 3 && gender == G_FEMALE) file = mFileMotion.GetString(motion, G_MALE);
	if(strlen(file) < 3) return SmartPointer<ROSE::ZMO>();

	return ANIM_MGR().Load(file);
}
