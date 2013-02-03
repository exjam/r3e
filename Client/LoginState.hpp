#ifndef LOGIN_STATE_HPP
#define LOGIN_STATE_HPP

#include "..\R3E\Text.hpp"
#include "..\R3E\PathCamera.hpp"
#include "..\R3E\AvatarEntity.hpp"
#include "..\R3E\SceneManager.hpp"

#include "ToolTip.hpp"
#include "LoginDialog.hpp"
#include "StateManager.hpp"
#include "ServerListDialog.hpp"
#include "CharacterSelectDialog.hpp"

enum CameraStopID {
	CAMERA_IN_SELECT_CHAR = 1,
};

enum LoginResult {
	LOGIN_OK,
	LOGIN_FAILED,
	LOGIN_BAD_ACCOUNT,
	LOGIN_BAD_PASSWORD,
	LOGIN_ALREADY_LOGGEDIN,
	LOGIN_BLOCKED,
	LOGIN_NEED_PAY,
	LOGIN_NO_RIGHT,
	LOGIN_TOO_MANY_USER,
	LOGIN_MAIL_NOT_CONFIRM,
	LOGIN_INVALID_VERSION,
	LOGIN_OUT_OF_IP,
};

static const char* gLoginStrs[] = {
	"Login OK",
	"Login failed",
	"Invalid account",
	"Invalid password",
	"Already logged in",
	"Login blocked",
	"P2P fail.",
	"Account banned",
	"Server busy",
	"Email address unconfirmed",
	"Incorrect version",
	"Server busy (2)",
	0
};

enum ServerSelectResult {
	SELECT_OK,
	SELECT_FAILED,
	SELECT_FULL,
	SELECT_INVALID_CHANNEL,
	SELECT_INACTIVE_CHANNEL,
	SELECT_INVALID_AGE,
	SELECT_NEED_PAY,
};

static const char* gServerSelectStrs[] = {
	"Select OK",
	"Channel full",
	"Invalid channel",
	"Inactive channel",
	"Invalid age",
	"P2P fail.",
	0
};

enum JoinResult {
	JOIN_OK,
	JOIN_FAILED,
	JOIN_TIME_OUT,
	JOIN_INVALID_PASSWORD,
	JOIN_ALREADY_LOGGEDIN,
};

static const char* gJoinResultStrs[] = {
	"Join OK",
	"Join failed",
	"Join timeout",
	"Invalid password",
	"Already loggedin",
	0
};

struct SelectAvatar {
	SelectAvatar() : mEntity(0), mToolTip(0) {}
	~SelectAvatar(){
		gScene->RemoveEntity(mEntity);
		SAFE_DELETE(mEntity);
		SAFE_DELETE(mToolTip);
	}

	String mName;
	int mLevel;
	int mClass;
	unsigned int mDeleteTime;
	bool mIsPlatinum;
	Avatar* mEntity;
	ToolTip* mToolTip;
};

class LoginState : public GameState {
public:
	LoginState();
	virtual ~LoginState();

	virtual bool CanEnterState();
	virtual void EnterState();
	virtual bool LeaveState();

	virtual int HandleEvent(GuiEvent* gevt);
	virtual int HandleEvent(GameEvent* gevt);
	virtual int HandleEvent(NetworkEvent* nevt);

	virtual int Run();

private:
	void RequestChannelList(int srv);
	void SetSelectedCharacter(unsigned int idx);

private:
	Text mVersionText;
	LoginDialog* mLoginDialog;
	ServerListDialog* mServerListDialog;
	CharacterSelectDialog* mCharacterSelectDialog;

	String mUsername;
	String mPasswordHash;

	int mUserID;

	PathCamera* mCamera;

	int mWaitCameraFinish;

	int mCurMsgBoxSrv;

	Array<SelectAvatar, 5> mAvatars;
	int mSelectedCharacter;

	bool mRenderCharacterTips;
};


#endif