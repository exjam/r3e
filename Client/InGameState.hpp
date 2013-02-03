#ifndef IN_GAME_STATE_HPP
#define IN_GAME_STATE_HPP

#include "StateManager.hpp"

class InGameState : public GameState {
public:
	InGameState();
	virtual ~InGameState();

	virtual bool CanEnterState();
	virtual void EnterState();
	virtual bool LeaveState();

	virtual int HandleEvent(GuiEvent* gevt);
	virtual int HandleEvent(GameEvent* gevt);
	virtual int HandleEvent(NetworkEvent* nevt);

	virtual int Run();

	void DeleteObject(unsigned int cid);

private:
	class TargetCamera* mCamera;
};

#endif
