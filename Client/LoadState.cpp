#include "Global.h"

#include "Fonts.hpp"
#include "Player.hpp"
#include "LoadState.hpp"
#include "UserInterface.hpp"
#include "CharacterList.hpp"

#include "..\R3E\RoseData.hpp"
#include "..\R3E\MapManager.hpp"
#include "..\R3E\SceneManager.hpp"

LoadState::LoadState(int map){
	mLoadMap = map;
}

LoadState::~LoadState(){
}

bool LoadState::CanEnterState(){
	return true;
}

void LoadState::EnterState(){
}

bool LoadState::LeaveState(){
	return true;
}

int LoadState::HandleEvent(GuiEvent* /*gevt*/){
	return 0;
}

int LoadState::HandleEvent(GameEvent* /*gevt*/){
	printf("LoadState::HandleGameEvent should never be called!\n");
	return 0;
}

int LoadState::HandleEvent(NetworkEvent* /*nevt*/){
	printf("LoadState::HandleNetworkEvent should never be called!\n");
	return 0;
}

int LoadState::Run(){
	static bool sFirstLoad = true;
	
	if(sFirstLoad){
		gScene->Init();

		ROSE::Data::Load();
		GameFonts::Load();
		gInterface->Init();
		sFirstLoad = false;
	}

	if(gPlayer)
		gScene->RemoveEntity(gPlayer);//No deletion of us thanks

	gScene->ClearScene();
	gCharacterList->Clear();

	Map* map = 0;
	SkyBox* skybox = 0;
	if(!MapManager::Instance().LoadMap(mLoadMap, skybox, map)) return -1;

	gScene->SetMap(map);
	gScene->SetSkyBox(skybox);
	return 0;
}
