#include "Global.h"

#include "ItemManager.hpp"

ItemManager ItemManager::mInstance;

ItemManager::ItemManager(){}
ItemManager::~ItemManager(){
	mItemModels[G_MALE].DeleteValues();

	mItemModels[G_FEMALE][IT_MASK] = 0;
	mItemModels[G_FEMALE][IT_BACK] = 0;
	mItemModels[G_FEMALE][IT_WEAPON] = 0;
	mItemModels[G_FEMALE][IT_SUBWPN] = 0;
	mItemModels[G_FEMALE][IT_PAT] = 0;
	mItemModels[G_FEMALE].DeleteValues();

	mItemSTL.DeleteValues();
	mItemSTB.DeleteValues();
}

void ItemManager::Load(){
	mItemModels[G_MALE].SetSize(IT_MAX);
	mItemModels[G_FEMALE].SetSize(IT_MAX);

	memset(mItemModels[G_MALE].Data(), 0, sizeof(ROSE::ZSC<Mesh1TexEntity>*) * IT_MAX);
	memset(mItemModels[G_FEMALE].Data(), 0, sizeof(ROSE::ZSC<Mesh1TexEntity>*) * IT_MAX);

	mItemModels[G_FEMALE][IT_MASK] = mItemModels[G_MALE][IT_MASK] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_FACEIEM.ZSC");
	mItemModels[G_FEMALE][IT_BACK] = mItemModels[G_MALE][IT_BACK] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_BACK.ZSC");

	mItemModels[G_FEMALE][IT_WEAPON] = mItemModels[G_MALE][IT_WEAPON] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\WEAPON\\LIST_WEAPON.ZSC");
	mItemModels[G_FEMALE][IT_SUBWPN] = mItemModels[G_MALE][IT_SUBWPN] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\WEAPON\\LIST_SUBWPN.ZSC");

	mItemModels[G_FEMALE][IT_PAT] = mItemModels[G_MALE][IT_PAT] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\PAT\\LIST_PAT.ZSC");

	mItemModels[G_MALE][IT_CAP] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MCAP.ZSC");
	mItemModels[G_MALE][IT_BODY] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MBODY.ZSC");
	mItemModels[G_MALE][IT_ARM] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MARMS.ZSC");
	mItemModels[G_MALE][IT_FOOT] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MFOOT.ZSC");
	mItemModels[G_MALE][IT_FACE] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MFACE.ZSC");
	mItemModels[G_MALE][IT_HAIR] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_MHAIR.ZSC");

	mItemModels[G_FEMALE][IT_CAP] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WCAP.ZSC");
	mItemModels[G_FEMALE][IT_BODY] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WBODY.ZSC");
	mItemModels[G_FEMALE][IT_ARM] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WARMS.ZSC");
	mItemModels[G_FEMALE][IT_FOOT] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WFOOT.ZSC");
	mItemModels[G_FEMALE][IT_FACE] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WFACE.ZSC");
	mItemModels[G_FEMALE][IT_HAIR] = new ROSE::ZSC<Mesh1TexEntity>("3DDATA\\AVATAR\\LIST_WHAIR.ZSC");

	mItemModels[G_MALE][IT_BACK]->SetBindDummy(3);

	mItemModels[G_MALE][IT_MASK]->SetBindDummy(4);

	mItemModels[G_MALE][IT_CAP]->SetBindDummy(6);
	mItemModels[G_FEMALE][IT_CAP]->SetBindDummy(6);

	mItemModels[G_MALE][IT_FACE]->SetBindBone(4);
	mItemModels[G_MALE][IT_HAIR]->SetBindBone(4);

	mItemModels[G_FEMALE][IT_FACE]->SetBindBone(4);
	mItemModels[G_FEMALE][IT_HAIR]->SetBindBone(4);

	mItemSTB.SetSize(IT_MAX);
	mItemSTL.SetSize(IT_MAX);

	memset(mItemSTB.Data(), 0, sizeof(ROSE::STB*) * IT_MAX);
	memset(mItemSTL.Data(), 0, sizeof(ROSE::STL*) * IT_MAX);

	mItemSTB[IT_MASK] = new ROSE::STB("3DDATA\\STB\\LIST_FACEITEM.STB");
	mItemSTB[IT_CAP] = new ROSE::STB("3DDATA\\STB\\LIST_CAP.STB");
	mItemSTB[IT_BODY] = new ROSE::STB("3DDATA\\STB\\LIST_BODY.STB");
	mItemSTB[IT_ARM] = new ROSE::STB("3DDATA\\STB\\LIST_ARMS.STB");
	mItemSTB[IT_FOOT] = new ROSE::STB("3DDATA\\STB\\LIST_FOOT.STB");
	mItemSTB[IT_BACK] = new ROSE::STB("3DDATA\\STB\\LIST_BACK.STB");
	mItemSTB[IT_JEWEL] = new ROSE::STB("3DDATA\\STB\\LIST_JEWEL.STB");
	mItemSTB[IT_WEAPON] = new ROSE::STB("3DDATA\\STB\\LIST_WEAPON.STB");
	mItemSTB[IT_SUBWPN] = new ROSE::STB("3DDATA\\STB\\LIST_SUBWPN.STB");
	mItemSTB[IT_USE] = new ROSE::STB("3DDATA\\STB\\LIST_USEITEM.STB");
	mItemSTB[IT_GEM] = new ROSE::STB("3DDATA\\STB\\LIST_JEMITEM.STB");
	mItemSTB[IT_NATURAL] = new ROSE::STB("3DDATA\\STB\\LIST_NATURAL.STB");
	mItemSTB[IT_QUEST] = new ROSE::STB("3DDATA\\STB\\LIST_QUESTITEM.STB");
	mItemSTB[IT_PAT] = new ROSE::STB("3DDATA\\STB\\LIST_PAT.STB");
	mItemSTB[IT_FACE] = new ROSE::STB("3DDATA\\STB\\LIST_FACE.STB");
	mItemSTB[IT_HAIR] = new ROSE::STB("3DDATA\\STB\\LIST_HAIR.STB");

	mItemSTL[IT_MASK] = new ROSE::STL("3DDATA\\STB\\LIST_FACEITEM_S.STL");
	mItemSTL[IT_CAP] = new ROSE::STL("3DDATA\\STB\\LIST_CAP_S.STL");
	mItemSTL[IT_BODY] = new ROSE::STL("3DDATA\\STB\\LIST_BODY_S.STL");
	mItemSTL[IT_ARM] = new ROSE::STL("3DDATA\\STB\\LIST_ARMS_S.STL");
	mItemSTL[IT_FOOT] = new ROSE::STL("3DDATA\\STB\\LIST_FOOT_S.STL");
	mItemSTL[IT_BACK] = new ROSE::STL("3DDATA\\STB\\LIST_BACK_S.STL");
	mItemSTL[IT_JEWEL] = new ROSE::STL("3DDATA\\STB\\LIST_JEWEL_S.STL");
	mItemSTL[IT_WEAPON] = new ROSE::STL("3DDATA\\STB\\LIST_WEAPON_S.STL");
	mItemSTL[IT_SUBWPN] = new ROSE::STL("3DDATA\\STB\\LIST_SUBWPN_S.STL");
	mItemSTL[IT_USE] = new ROSE::STL("3DDATA\\STB\\LIST_USEITEM_S.STL");
	mItemSTL[IT_GEM] = new ROSE::STL("3DDATA\\STB\\LIST_JEMITEM_S.STL");
	mItemSTL[IT_NATURAL] = new ROSE::STL("3DDATA\\STB\\LIST_NATURAL_S.STL");
	mItemSTL[IT_QUEST] = new ROSE::STL("3DDATA\\STB\\LIST_QUESTITEM_S.STL");
	mItemSTL[IT_PAT] = new ROSE::STL("3DDATA\\STB\\LIST_PAT_S.STL");

	MotionManager::Instance().SetWeaponSTB(mItemSTB[IT_WEAPON]);
}

Entity* ItemManager::LoadItem(int id, int type, int gender, Entity* parent) const {
	return mItemModels[gender][type]->LoadModel(id, parent);
}

ROSE::STB* ItemManager::GetSTB(int type) const {
	return mItemSTB[type];
}

ROSE::STL* ItemManager::GetSTL(int type) const {
	return mItemSTL[type];
}