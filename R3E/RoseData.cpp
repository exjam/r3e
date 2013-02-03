#include "Global.h"

#include "ROSEData.hpp"
#include "NpcManager.hpp"
#include "MapManager.hpp"
#include "ItemManager.hpp"
#include "MotionManager.hpp"
#include "ResourceManager.hpp"

SmartPointer<ROSE::ZMD> ROSE::Data::mAvatarSkeletons[G_MAX];

namespace ROSE {
	void Data::Load(){
		static bool loaded = false;
		if(loaded) return;
		loaded = true;

		mAvatarSkeletons[G_MALE] = SKEL_MGR().Load("3DDATA\\AVATAR\\MALE.ZMD");
		mAvatarSkeletons[G_FEMALE] = SKEL_MGR().Load("3DDATA\\AVATAR\\FEMALE.ZMD");

		MapManager::Instance().Load();
		NpcManager::Instance().Load();
		ItemManager::Instance().Load();
		MotionManager::Instance().Load();
	}
};
