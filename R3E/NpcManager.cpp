#include "Global.h"

#include "NpcManager.hpp"

#include "ZSC.hpp"
#include "CHR.hpp"
#include "STB.hpp"
#include "STL.hpp"
#include "ZMD.hpp"
#include "ZMO.hpp"

#include "NpcEntity.hpp"
#include "Mesh1TexEntity.hpp"

NpcManager NpcManager::mInstance;

NpcManager::NpcManager() : mZSC(0), mCHR(0), mSTB(0), mSTL(0) {}
NpcManager::~NpcManager(){
	//SAFE_DELETE(mCHR);
	//SAFE_DELETE(mZSC);
	SAFE_DELETE(mSTL);
	SAFE_DELETE(mSTB);
}

void NpcManager::Load(){
	mCHR = new ROSE::CHR("3DDATA\\NPC\\LIST_NPC.CHR");
	mZSC = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\NPC\\PART_NPC.ZSC");
	mSTB = new ROSE::STB("3DDATA\\STB\\LIST_NPC.STB");
	mSTL = new ROSE::STL("3DDATA\\STB\\LIST_NPC_S.STL");
}

bool NpcManager::LoadNpcAnimation(int type, NpcEntity* npc) const {
	ROSE::CHR::Character* chr = mCHR->GetCharacter(npc->NpcID());
	if(!chr) return false;
	if(!chr->mActive) return false;
	ROSE::CHR::AnimationLink* animlink = chr->GetAnimation(type);
	if(!animlink) return false;
	ROSE::CHR::Animation* anim = mCHR->GetAnimation(animlink->mAnimation);
	if(!anim) return false;

	SmartPointer<ROSE::ZMO> lanim = ANIM_MGR().Load(anim->mPath);
	npc->SetAnimation(lanim);

	return true;
}

bool NpcManager::LoadNpcModel(NpcEntity* npc) const {
	ROSE::CHR::Character* chr = mCHR->GetCharacter(npc->NpcID());
	if(!chr) return false;
	if(!chr->mActive) return false;

	ROSE::CHR::Skeleton* skel = mCHR->GetSkeleton(chr->mSkeleton);
	if(!skel) return false;

	SmartPointer<ROSE::ZMD> lskel = SKEL_MGR().Load(skel->mPath);

	npc->SetSkeleton(lskel);
	npc->SetID(npc->NpcID());
	
	for(unsigned int i = 0; i < chr->mModelLinks.Size(); ++i)
		npc->AddChild(mZSC->LoadModel(chr->mModelLinks[i].mModel));

	return true;
}

unsigned int NpcManager::NpcCount() const {
	return mCHR->Characters();
}

namespace NpcStbColumns {
	enum NpcStbColumns {
		WalkSpeed = 2,
		RunSpeed = 3,
		Scale = 4,
		RWeapon = 5,
		LWeapon = 6,
		Level = 7,
		HP = 8,
		AttackPower = 9,
		Accuracy = 10,
		Defence = 11,
		MagicResistance = 12,
		DodgeRate = 13,
		AttackSpeed = 14,
		DamageType = 15,
		AIP = 16,
		Exp = 17,
		DropType = 18,
		DropMoney = 19,
		DropItem = 20,
		SellTab1 = 21,
		SellTab2 = 22,
		SellTab3 = 23,
		SellTab4 = 24,
		TargetType = 25,
		AttackRange = 26,
		CharacterType = 27,
		MaterialType = 28,
		FaceIcon = 29,
		NormalSound = 30,
		AttackSound = 31,
		DamagedSound = 32,
		MeleeEffect = 33,
		DieEffect = 34,
		DieSound = 35,
		QuestType = 38,
		GlowColour = 39,
		StringID = 40,
	};
};

const char* NpcManager::GetName(int npc) const {
	const char* strid = mSTB->GetString(npc, NpcStbColumns::StringID);
	if(!strid || memcmp(strid, "LNPC", 4) != 0) return NULL;
	int id = mSTL->GetIDByStr(strid);
	if(id == -1) return NULL;
	return mSTL->GetText(id);
}

int NpcManager::GetWalkSpeed(int npc) const {
	return mSTB->GetInt(npc, NpcStbColumns::WalkSpeed);
}

int NpcManager::GetRunSpeed(int npc) const {
	return mSTB->GetInt(npc, NpcStbColumns::RunSpeed);
}

int NpcManager::GetScale(int npc) const {
	return mSTB->GetInt(npc, NpcStbColumns::Scale);
}
