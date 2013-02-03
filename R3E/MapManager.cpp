#include "Global.h"

#include "MapManager.hpp"

#include "Map.hpp"
#include "SkyBox.hpp"
#include "Matrix4.hpp"
#include "Material.hpp"
#include "SceneManager.hpp"
#include "ObjectManager.hpp"

MapManager MapManager::mInstance;

MapManager::MapManager(){}
MapManager::~MapManager(){}

void MapManager::Load(){
	mSkyData.Load("3DDATA\\STB\\LIST_SKY.STB");
	mZoneData.Load("3DDATA\\STB\\LIST_ZONE.STB");
}

bool MapManager::LoadMap(int id, SkyBox*& _skybox, Map*& _map) const {
	const char* zon = mZoneData.GetString(id, 1);
	const char* deco = mZoneData.GetString(id, 11);
	const char* cnst = mZoneData.GetString(id, 12);

	if(!zon || strlen(zon) < 5) return false;
	if(!deco || strlen(deco) < 5) return false;
	if(!cnst || strlen(cnst) < 5) return false;
	
	int skybox = mZoneData.GetInt(id, 7);
	_skybox = CreateSkybox(skybox);

	ObjectManager::Instance().SetDeco(deco);
	ObjectManager::Instance().SetCnst(cnst);

	Map* map = new Map();
	map->Load(zon);
	_map = map;

	return true;
}

SkyBox* MapManager::CreateSkybox(int id) const {
	Material mat;
	mat.SetTexture(mSkyData.GetString(id, 1));
	mat.mAlphaTest = false;
	mat.mZWrite = false;
	mat.mZTest = false;

	SkyBox* sky = new SkyBox();
	sky->SetMesh(mSkyData.GetString(id, 0));
	sky->SetMaterial(mat);
	sky->SetDayTexture(mSkyData.GetString(id, 1));
	sky->SetNightTexture(mSkyData.GetString(id, 2));

	return sky;
}
