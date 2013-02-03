#ifndef NPCMAN_H
#define NPCMAN_H

#include "ResourceManager.hpp"

class Mesh1TexEntity;
class NpcEntity;

namespace ROSE {
	template<class T> class ZSC;
	class CHR;
	class STB;
	class STL;
};

class NpcManager {
private:
	static NpcManager mInstance;

public:
	NpcManager();
	~NpcManager();

	static NpcManager& Instance(){
		return mInstance;
	}

	void Load();

	bool LoadNpcModel(NpcEntity* npc) const;
	bool LoadNpcAnimation(int type, NpcEntity* npc) const;

	unsigned int NpcCount() const;

	int GetScale(int npc) const;
	int GetRunSpeed(int npc) const;
	int GetWalkSpeed(int npc) const;
	const char* GetName(int npc) const;

private:
	ROSE::ZSC<Mesh1TexEntity>* mZSC;
	ROSE::CHR* mCHR;
	ROSE::STB* mSTB;
	ROSE::STL* mSTL;
};

#define NPC_MAN() NpcManager::Instance()

#endif