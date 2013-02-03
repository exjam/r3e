#include "Global.h"

#include "MD5.hpp"
#include "Fonts.hpp"
#include "Player.hpp"
#include "Window.hpp"
#include "Network.hpp"
#include "LoadState.hpp"
#include "LoginState.hpp"
#include "GameEvents.hpp"
#include "DialogType.hpp"
#include "InGameState.hpp"
#include "UserInterface.hpp"

#include "..\RGE\ListBox.hpp"
#include "..\R3E\FontList.hpp"
#include "..\R3E\MapManager.hpp"
#include "..\R3E\SceneManager.hpp"
#include "..\RNE\ConnectEvent.hpp"
#include "..\R3E\ResourceManager.hpp"

LoginState::LoginState()
  : mLoginDialog(0), mCamera(0), mWaitCameraFinish(0), mCurMsgBoxSrv(LS), mSelectedCharacter(-1), mRenderCharacterTips(false)
{
}

LoginState::~LoginState(){
}

bool LoginState::CanEnterState(){
	return true;
}

void LoginState::EnterState(){
	mVersionText.SetFont(FontList::Instance().GetFont(FONT_NORMAL_OUTLINE));
	mVersionText.SetText("A lovely ROSE client by ExJam");

	mLoginDialog = (LoginDialog*)gInterface->FindDialog(DLG_LOGIN);
	mServerListDialog = (ServerListDialog*)gInterface->FindDialog(DLG_SERVER_LIST);
	mCharacterSelectDialog = (CharacterSelectDialog*)gInterface->FindDialog(DLG_CHARACTER_SELECT);

	mLoginDialog->SetVisible(true);

	mCamera = new PathCamera();
	mCamera->SetPath(ANIM_MGR().Load("3DDATA\\TITLE\\CAMERA01_INTRO01.ZMO"));
	mCamera->SetRepeatCount(PathCamera::REPEAT_INFINITE);
	gScene->SetCamera(mCamera);
}

bool LoginState::LeaveState(){
	gScene->SetCamera(NULL);
	SAFE_DELETE(mCamera);

	mAvatars.Clear();

	mLoginDialog->SetVisible(false);
	mServerListDialog->SetVisible(false);
	mCharacterSelectDialog->SetVisible(false);

	mActive = false;
	return true;
}

int LoginState::HandleEvent(NetworkEvent* nevt){
	switch(nevt->_evt_type){
		case NET_DISCONNECT:
			gInterface->ShowMsgBox("Disconnected from server!", MsgBox::BUTTON_OK);
		break;
		case NET_CONNECT_RESULT:
		{
			ConnectEvent* evt = (ConnectEvent*)nevt;
			if(!evt->mSuccess){
				gInterface->ShowMsgBox("Connection failed.", MsgBox::BUTTON_OK);
				return 0;
			}

			if(evt->srvID == LS){
				gInterface->ShowMsgBox("Logging in...", MsgBox::BUTTON_CANCEL);

				Packet pakout(0x703);
				gNetwork->SendPacket(LS, &pakout);
			}else if(evt->srvID == WS){
				gInterface->ShowMsgBox("Logging in...", MsgBox::BUTTON_CANCEL);
				Packet pakout(0x70B);
				pakout.Add(mUserID);
				pakout.Add(mPasswordHash, 32);
				gNetwork->SendPacket(WS, &pakout);
			}else if(evt->srvID == GS){
				gInterface->ShowMsgBox("Logging in...", MsgBox::BUTTON_CANCEL);
				Packet pakout(0x70B);
				pakout.Add(mUserID);
				pakout.Add(mPasswordHash, 32);
				gNetwork->SendPacket(GS, &pakout);
			}
		}
		break;
		case NET_PAK_NET_STATUS:
		{
			PakNetStatusEvent* evt = (PakNetStatusEvent*)nevt;
			if(evt->mStatus != 2){
				gInterface->ShowMsgBox("Invalid network status.", MsgBox::BUTTON_OK);
				return 0;
			}

			gNetwork->SetCryptKey(evt->srvID, evt->mKey);

			Packet pakout(0x708);
			pakout.Add(mPasswordHash, 32);
			pakout.Add(mUsername);
			gNetwork->SendPacket(LS, &pakout);
		}
		break;
		case NET_PAK_LOGIN_RESULT:
		{
			PakLoginResultEvent* evt = (PakLoginResultEvent*)nevt;
			if(evt->mResult != LOGIN_OK){
				if(evt->mResult > LOGIN_OUT_OF_IP)
					evt->mResult = 1;

				gInterface->ShowMsgBox(gLoginStrs[evt->mResult], MsgBox::BUTTON_OK);
				gNetwork->Disconnect(1);
				return 0;
			}

			mServerListDialog->SetVisible(true);
			mLoginDialog->SetVisible(false);
			mCharacterSelectDialog->SetVisible(false);
			gInterface->HideMsgBox();

			mServerListDialog->ClearServers();
			for(unsigned int i = 0; i < evt->mServers.Size(); ++i)
				mServerListDialog->AddServer(&evt->mServers[i]);

			mServerListDialog->mServerList->SetSelected(0);

			if(evt->mServers.Size())
				RequestChannelList(evt->mServers[0].mID);
		}
		break;
		case NET_PAK_CHANNEL_LIST:
		{
			PakChannelListEvent* evt = (PakChannelListEvent*)nevt;
		
			mServerListDialog->mChannelList->SetSelected(0);
			mServerListDialog->ClearChannels();
			for(unsigned int i = 0; i < evt->mChannels.Size(); ++i)
				mServerListDialog->AddChannel(evt->mServer, &evt->mChannels[i]);
		}
		break;
		case NET_PAK_SELECT_SERVER:
		{
			PakSelectServerEvent* evt = (PakSelectServerEvent*)nevt;

			if(evt->mResult != SELECT_OK){
				gInterface->ShowMsgBox(gServerSelectStrs[evt->mResult], MsgBox::BUTTON_OK);
			}else{
				mUserID = evt->mUserID;
				mCurMsgBoxSrv = WS;
				gNetwork->Connect(WS, evt->mIP, evt->mPort, evt->mCryptSeed);
				gInterface->ShowMsgBox("Connecting...", MsgBox::BUTTON_CANCEL);
			}
		}
		break;
		case NET_PAK_SERVER_JOIN:
		{
			PakServerJoinEvent* evt = (PakServerJoinEvent*)nevt;
			if(evt->mResult != JOIN_OK){
				gInterface->ShowMsgBox(gJoinResultStrs[evt->mResult], MsgBox::BUTTON_OK);
				gNetwork->Disconnect(evt->srvID);
			}else{
				gNetwork->SetCryptKey(evt->srvID, evt->mKey);
				if(evt->srvID == WS){
					gInterface->ShowMsgBox("Getting character list...", MsgBox::BUTTON_CANCEL);
					Packet pakout(0x712);
					gNetwork->SendPacket(WS, &pakout);
				}else if(evt->srvID == GS){
					gInterface->ShowMsgBox("Joining game...", MsgBox::BUTTON_CANCEL);
				}
			}
		}
		break;
		case NET_PAK_CHARACTER_LIST:
		{
			PakCharacterListEvent* evt = (PakCharacterListEvent*)nevt;

			mServerListDialog->SetVisible(false);
			mLoginDialog->SetVisible(false);
			gInterface->HideMsgBox();

			mCamera->SetPath(ANIM_MGR().Load("3DDATA\\TITLE\\CAMERA01_INSELECT01.ZMO"));
			mCamera->SetRepeatCount(1);
			mWaitCameraFinish = CAMERA_IN_SELECT_CHAR;

			static Vector3 AvatarPositions[] = {
				Vector3(5205.00f, 5205.00f, 1.0f),
				Vector3(5202.70f, 5206.53f, 1.0f),
				Vector3(5200.00f, 5207.07f, 1.0f),
				Vector3(5197.30f, 5206.53f, 1.0f),
				Vector3(5195.00f, 5205.00f, 1.0f),
			};

			for(unsigned int i = 0; i < evt->mCharacters.Size(); ++i){
				SelectAvatar* avatar = &mAvatars.Add();
				PakCharacterListEvent::Character* chr = &evt->mCharacters[i];
				avatar->mName = chr->mName;
				avatar->mLevel = chr->mLevel;
				avatar->mClass = chr->mClass;
				avatar->mDeleteTime = chr->mDeleteTime;
				avatar->mIsPlatinum = chr->mIsPlatinum;
				
				ToolTip* tip = new ToolTip();
				avatar->mToolTip = tip;
				tip->AddLine(avatar->mName, 0, Colour::Yellow, ALIGN_CENTER);
				tip->AddLine(String("Level: %1").arg(avatar->mLevel));
				tip->AddLine(String("Class: %1").arg(avatar->mClass));

				Avatar* entity = new Avatar();
				avatar->mEntity = entity;
				entity->SetGender(chr->mRace);
				entity->SetFaceStyle(chr->mItems[BP_FACE].mItemNo);
				entity->SetHairStyle(chr->mItems[BP_HAIR].mItemNo);
				entity->SetState(STATE_STOP);
				entity->SetItems(chr->mItems);
				entity->SetDirection(M_PI);
				entity->SetPosition(AvatarPositions[i]);
				entity->SetScale(1.5f);
				gScene->AddEntity(entity);
			}
		}
		break;
		case NET_PAK_GAME_SERVER_IP:
		{
			PakGameServerIPEvent* evt = (PakGameServerIPEvent*)nevt;
			mUserID = evt->mUserID;
			mCurMsgBoxSrv = GS;
			gNetwork->Connect(GS, evt->mIP, evt->mPort, evt->mCryptSeed);
			gInterface->ShowMsgBox("Connecting...", MsgBox::BUTTON_CANCEL);
		}
		break;
		case NET_PAK_PLAYER_DATA:
		{
			PakPlayerDataEvent* evt = (PakPlayerDataEvent*)nevt;
			
			printf("Join map %d Position: %f %f\n", evt->mData->mZone, evt->mData->mPosition);
			SAFE_DELETE(gPlayer);

			gPlayer = new Player();
			gPlayer->SetName(evt->mName);
			gPlayer->SetGender(evt->mData->mGender);
			gPlayer->SetItems(evt->mData->mEquipment);
			gPlayer->SetFaceStyle(evt->mData->mBasicInfo.mFaceIDX);
			gPlayer->SetHairStyle(evt->mData->mBasicInfo.mHairIDX);
			gPlayer->SetPosition(evt->mData->mPosition);

			gStateManager->EnqueueState(new LoadState(evt->mData->mZone));
			gStateManager->EnqueueState(new InGameState());

			gInterface->HideMsgBox();
			LeaveState();
		}
		break;
	}

	return 0;
}

void LoginState::SetSelectedCharacter(unsigned int idx){
	mSelectedCharacter = idx;
	mAvatars[mSelectedCharacter].mEntity->SetAvatarState(AVT_ANI_JUMP1);
	for(unsigned int i = 0; i < mAvatars.Size(); ++i){
		if(i == idx) continue;
		mAvatars[i].mEntity->SetState(STATE_STOP);
	}
}

int LoginState::HandleEvent(GuiEvent* gevt){
	if(gevt->_evt_type == EVT_WINDOW && mLoginDialog){
		WindowEvent* evt = (WindowEvent*)gevt;
		
		mLoginDialog->SetPositionX(evt->width - mLoginDialog->SizeX() - 50);
		mLoginDialog->SetPositionY(100);
		return 0;
	}else{
		int ret = gInterface->HandleEvent(gevt);
		if(gevt->_evt_type == EVT_MOUSE && !ret && mAvatars.Size()){
			MouseEvent* evt = (MouseEvent*)gevt;
			if(evt->type == BUTTON_DOWN && evt->button == LBUTTON){
				gScene->MouseMove(evt->pos.x, evt->pos.y, 0);
				Entity* entity = gScene->GetHoverEntity();
				for(unsigned int i = 0; i < mAvatars.Size(); ++i){
					if(entity != mAvatars[i].mEntity) continue;
					SetSelectedCharacterEvent* evt = gGameEvents->CreateEvent<SetSelectedCharacterEvent>();
					evt->mCharacterIdx = i;
					gGameEvents->Emit(evt);
					return 1;
				}
			}else if(evt->type == BUTTON_DBLCLICK && evt->button == LBUTTON){
				gScene->MouseMove(evt->pos.x, evt->pos.y, 0);
				Entity* entity = gScene->GetHoverEntity();
				for(unsigned int i = 0; i < mAvatars.Size(); ++i){
					if(entity != mAvatars[i].mEntity) continue;
					SelectCharacterEvent* evt = gGameEvents->CreateEvent<SelectCharacterEvent>();
					evt->mCharacterIdx = i;
					gGameEvents->Emit(evt);
					return 1;
				}
			}
		}

		return ret;
	}
}

int LoginState::HandleEvent(GameEvent* gevt){
	if(gevt->_evt_type == GE_LOGIN){
		LoginEvent* evt = (LoginEvent*)gevt;

		mUsername = evt->mUsername;
		mPasswordHash = md5(evt->mPassword);

		gNetwork->Connect(LS, "174.37.234.130", 29000, 0);
		gInterface->ShowMsgBox("Connecting...", MsgBox::BUTTON_CANCEL);
	}else if(gevt->_evt_type == GE_LOGOUT){
		gNetwork->Disconnect(1);
		mServerListDialog->SetVisible(false);
		mCharacterSelectDialog->SetVisible(false);
		mLoginDialog->SetVisible(true);
		gInterface->HideMsgBox();
	}else if(gevt->_evt_type == GE_SELECT_SERVER){
		SelectServerEvent* evt = (SelectServerEvent*)gevt;

		Packet pakout(0x70A);
		pakout.Add<int>(evt->mServer);
		pakout.AddVal<char>(char(evt->mChannel));
		gNetwork->SendPacket(LS, &pakout);

		gInterface->ShowMsgBox("Requesting server...", MsgBox::BUTTON_CANCEL);
	}else if(gevt->_evt_type == GE_CAMERA_STOP){
		mCharacterSelectDialog->SetVisible(true);
		mRenderCharacterTips = true;
	}else if(gevt->_evt_type == GE_SET_SELECTED_CHAR){
		SetSelectedCharacterEvent* evt = (SetSelectedCharacterEvent*)gevt;
		SetSelectedCharacter(evt->mCharacterIdx);
	}else if(gevt->_evt_type == GE_SELECT_CHAR){
		SelectCharacterEvent* evt = (SelectCharacterEvent*)gevt;
		
		Packet pakout(0x715);
		pakout.AddVal<char>(char(evt->mCharacterIdx));
		pakout.AddVal<short>(0);
		pakout.Add(mAvatars[evt->mCharacterIdx].mName);
		gNetwork->SendPacket(WS, &pakout);

		mCurMsgBoxSrv = WS;
		gInterface->ShowMsgBox("Requesting server...", MsgBox::BUTTON_CANCEL);
	}else if(gevt->_evt_type == GE_MSGBOX){
		MsgBoxEvent* evt = (MsgBoxEvent*)gevt;
		if(evt->mStatus == MsgBox::BUTTON_CANCEL)
			gNetwork->Disconnect(mCurMsgBoxSrv);
	}else if(gevt->_evt_type == GE_QUIT){
		gWindow->Close();
	}

	return 0;
}

void LoginState::RequestChannelList(int srv){
	Packet pakout(0x704);
	pakout.Add(srv);
	gNetwork->SendPacket(LS, &pakout);
}

int LoginState::Run(){
	mActive = true;

	while(mActive){
		gScene->BeginScene();
		gScene->RenderScene();
		gScene->EndScene();
		
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);

		gScene->Begin2D();
		mVersionText.Render();
		gInterface->Render();

		if(mRenderCharacterTips && mSelectedCharacter != -1){
			SelectAvatar* avatar = &mAvatars[mSelectedCharacter];
			Vector3 wpos = avatar->mEntity->GetPosition();
			wpos.z += avatar->mEntity->GetEntityHeight();
			Vector3 vpos = gScene->GetScreenPosition(wpos);
			Point pos(int(vpos.x), int(vpos.y));
			pos.x -= avatar->mToolTip->SizeX() / 2;
			pos.y -= avatar->mToolTip->SizeY();
			avatar->mToolTip->SetPosition(pos);
			avatar->mToolTip->Render();
		}

		gScene->End2D();

		gScene->UpdateScene();

		if(mWaitCameraFinish && mCamera->IsFinished()){
			CameraStopEvent* evt = gGameEvents->CreateEvent<CameraStopEvent>();
			evt->mStatus = mWaitCameraFinish;
			gGameEvents->Emit(evt);
			mWaitCameraFinish = 0;
		}

		gNetwork->Process();
		gGameEvents->Process();

		gWindow->SwapBuffers();
	}

	return 0;
}
