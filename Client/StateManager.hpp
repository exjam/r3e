#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "Array.hpp"
#include "Window.hpp"
#include "GameState.hpp"
#include "CriticalSection.hpp"

extern class GameState* gActiveState;
extern class StateManager* gStateManager;
extern class SceneManager* gScene;

class StateManager {
public:
	StateManager(){}
	~StateManager(){}

	bool EnqueueState(GameState* state){
		mStateLock.Enter();
		mStateQueue.PushBack(state);
		mStateLock.Leave();
		return true;
	}

	void WaitEnd(){
		mActive = false;
		if(gActiveState)
			gActiveState->LeaveState();

		WaitForSingleObject(mRunThread, 500);
	}
	
	int Run(){
		mActive = true;
		mRunThread = GetCurrentThread();
		while(mActive){
			mStateLock.Enter();
			if(mStateQueue.Size() == 0){
				mStateLock.Leave();
				Sleep(100);
				printf("Waiting for a valid state to enter...\n");
				continue;
			}

			gActiveState = mStateQueue[0];
			mStateQueue.Erase(0);
			mStateLock.Leave();

			gActiveState->EnterState();
			gActiveState->Run();
			SAFE_DELETE(gActiveState);
		}

		gActiveState = 0;
		return 0;
	}

private:
	volatile bool mActive;
	HANDLE mRunThread;
	CriticalSection mStateLock;
	Array<GameState*, 10> mStateQueue;
};


#endif