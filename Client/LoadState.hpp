#ifndef LOAD_STATE_HPP
#define LOAD_STATE_HPP

#include "StateManager.hpp"

class LoadState : public GameState {
public:
	LoadState(int map);
	virtual ~LoadState();

	virtual bool CanEnterState();
	virtual void EnterState();
	virtual bool LeaveState();

	virtual int HandleEvent(GuiEvent* gevt);
	virtual int HandleEvent(GameEvent* gevt);
	virtual int HandleEvent(NetworkEvent* nevt);

	virtual int Run();

private:
	int mLoadMap;
};

#endif
