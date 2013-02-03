#ifndef MOTIONMAN_H
#define MOTIONMAN_H

#include "STB.hpp"
#include "ZMO.hpp"
#include "ResourceManager.hpp"

class MotionManager {
private:
	static MotionManager mInstance;

public:
	MotionManager();
	~MotionManager();

	static MotionManager& Instance(){
		return mInstance;
	}

	void Load();

	void SetWeaponSTB(ROSE::STB* stb);
	SmartPointer<ROSE::ZMO> LoadMotion(int weapon, int action, int gender) const;

private:
	ROSE::STB mTypeMotion;
	ROSE::STB mFileMotion;
	ROSE::STB* mWeaponSTB;
};

#endif