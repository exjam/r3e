#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

#include "EventQueue.hpp"
#include "..\R3E\Vector3.hpp"

#define GAMEEVENT_DATA_SIZE 64

enum GameEventType {
	GE_QUIT,
	GE_LOGIN,
	GE_LOGOUT,
	GE_MSGBOX,
	GE_SELECT_SERVER,
	GE_CAMERA_STOP,
	GE_SET_SELECTED_CHAR,
	GE_SELECT_CHAR,
};

struct GameEvent {
	int _evt_type;
};

struct GameEventData : public GameEvent {
	char data[GAMEEVENT_DATA_SIZE];
};

struct QuitEvent : public GameEvent {
	static const unsigned int EventType = GE_QUIT;

	int mReserved;
};

struct LoginEvent : public GameEvent {
	static const unsigned int EventType = GE_LOGIN;

	const char* mUsername;
	const char* mPassword;
};

struct LogoutEvent : public GameEvent {
	static const unsigned int EventType = GE_LOGOUT;
};

struct MsgBoxEvent : public GameEvent {
	static const unsigned int EventType = GE_MSGBOX;

	int mStatus;
};

struct SelectServerEvent : public GameEvent {
	static const unsigned int EventType = GE_SELECT_SERVER;

	int mServer;
	int mChannel;
};

struct CameraStopEvent : public GameEvent {
	static const unsigned int EventType = GE_CAMERA_STOP;

	int mStatus;
};

struct SetSelectedCharacterEvent : public GameEvent {
	static const unsigned int EventType = GE_SET_SELECTED_CHAR;

	int mCharacterIdx;
};

struct SelectCharacterEvent : public GameEvent {
	static const unsigned int EventType = GE_SELECT_CHAR;

	int mCharacterIdx;
};

#endif