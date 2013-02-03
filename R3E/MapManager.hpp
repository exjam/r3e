#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include "STB.hpp"

class MapManager {
private:
	static MapManager mInstance;

public:
	MapManager();
	~MapManager();

	static MapManager& Instance(){
		return mInstance;
	}
	
	void Load();

	class SkyBox* CreateSkybox(int id) const;
	bool LoadMap(int id, class SkyBox*& skybox, class Map*& map) const;

private:
	ROSE::STB mSkyData;
	ROSE::STB mZoneData;
};

#endif
