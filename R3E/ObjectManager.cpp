#include "Global.h"

#include "ZSC.hpp"
#include "IFO.hpp"
#include "ObjectManager.hpp"
#include "Mesh1TexEntity.hpp"

ObjectManager ObjectManager::mInstance;

ObjectManager::ObjectManager() : mDeco(0), mCnst(0) {}
ObjectManager::~ObjectManager(){
	SAFE_DELETE(mDeco);
	SAFE_DELETE(mCnst);
}

void ObjectManager::SetCnst(const char* path){
	SAFE_DELETE(mCnst);
	mCnst = new ROSE::ZSC<Mesh1TexEntity>(path);
}

void ObjectManager::SetDeco(const char* path){
	SAFE_DELETE(mDeco);
	mDeco = new ROSE::ZSC<Mesh1TexEntity>(path);
}

EntityGroup* ObjectManager::LoadObject(int type, int id) const {
	if(mDeco && type == ROSE::IFO::DECORATIONS) return mDeco->LoadModel(id);
	if(mCnst && type == ROSE::IFO::BUILDINGS)   return mCnst->LoadModel(id);
	return NULL;
}
