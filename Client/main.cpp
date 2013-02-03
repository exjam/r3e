#include "Global.h"

#include "..\R3E\SceneManager.hpp"
#include "..\RNE\NetworkManager.hpp"

#include "Player.hpp"
#include "Window.hpp"
#include "Network.hpp"
#include "LoadState.hpp"
#include "GameEvents.hpp"
#include "LoginState.hpp"
#include "StateManager.hpp"
#include "UserInterface.hpp"
#include "CharacterList.hpp"
#include "TitanFileSystem.hpp"

Player* gPlayer = 0;
GameState* gActiveState = 0;
Window* gWindow = new Window();
Network* gNetwork = new Network();
SceneManager* gScene = new SceneManager();
GameEvents* gGameEvents = new GameEvents();
UserInterface* gInterface = new UserInterface();
StateManager* gStateManager = new StateManager();
CharacterList* gCharacterList = new CharacterList();

DWORD WINAPI StateManRun(LPVOID /*param*/){
	gWindow->SetOpenGLThread();
	OpenGL::InitGL();
	return gStateManager->Run();
}

DWORD WINAPI NetManRun(LPVOID /*param*/){
	return gNetwork->Run();
}

#ifndef _DEBUG
void __cdecl purecallHandler(void){
	DebugBreak();
	printf("WE IN purecallHandler\n");
}
#endif

int __cdecl main(int, char**){
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
	_set_purecall_handler(purecallHandler);
#endif
	LOGMODE(LOG_CONSOLE);

	TitanFileSystem* fs = new TitanFileSystem("D:\\Games\\TitanROSE\\");
	fs->LoadIndex("data.idx");
	FileSystem::SetFileSystem(fs);

	gWindow->SetSize(1024, 768);
	gWindow->SetTitle("ROSE++ by ExJam");
	gWindow->Create();

	gStateManager->EnqueueState(new LoadState(4));
	gStateManager->EnqueueState(new LoginState());

	CreateThread(NULL, 0, &StateManRun, 0, 0, 0);
	CreateThread(NULL, 0, &NetManRun, 0, 0, 0);

	int ret = gWindow->Run();
	gStateManager->WaitEnd();
	gNetwork->WaitEnd();

	gWindow->SetOpenGLThread();

	SAFE_DELETE(gScene);
	SAFE_DELETE(fs);
	SAFE_DELETE(gNetwork);
	SAFE_DELETE(gGameEvents);
	SAFE_DELETE(gInterface);
	SAFE_DELETE(gStateManager);
	SAFE_DELETE(gCharacterList);

	gWindow->Destroy();
	SAFE_DELETE(gWindow);

	return ret;
}
