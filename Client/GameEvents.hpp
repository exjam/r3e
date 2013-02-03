#ifndef GAME_EVENTS_HPP
#define GAME_EVENTS_HPP

#include "GameEvent.hpp"
#include "StateManager.hpp"

extern class GameEvents* gGameEvents;

#ifdef _DEBUG
#undef new
#endif

class GameEvents {
public:
	GameEvents(){}
	~GameEvents(){}

	template<class T> T* CreateEvent(){
		GameEvent* evt = mEvents.CreateEvent();
		T* val = ::new (evt) T();
		val->_evt_type = T::EventType;
		return val;
	}

	void Emit(GameEvent* evt){
		mEvents.Enqueue((GameEventData*)evt);
	}

	void Process(){
		if(!mEvents.StartPoll()) return;

		while(GameEvent* evt = mEvents.Poll())
			gActiveState->HandleEvent(evt);

		mEvents.EndPoll();
	}

private:
	EventQueue<GameEventData> mEvents;
};

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif
