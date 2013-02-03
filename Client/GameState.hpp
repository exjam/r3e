#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "..\RGE\GuiEvent.hpp"
#include "..\RNE\NetworkEvent.hpp"
#include "GameEvent.hpp"

class GameState {
public:
	virtual ~GameState(){}

	virtual bool CanEnterState() = 0;
	virtual bool LeaveState() = 0;

	virtual void EnterState() = 0;

	virtual int HandleEvent(GuiEvent* gevt) = 0;
	virtual int HandleEvent(GameEvent* gevt) = 0;
	virtual int HandleEvent(NetworkEvent* nevt) = 0;

	virtual int Run() = 0;

	bool IsActive(){
		return mActive;
	}

protected:
	friend class StateManager;

	void SetActive(bool active){
		mActive = active;
	}
	
	volatile bool mActive;
};

#endif