#include "Global.h"

#include "Npc.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include "Network.hpp"
#include "LoadState.hpp"
#include "DialogType.hpp"
#include "GameEvents.hpp"
#include "ChatDialog.hpp"
#include "InGameState.hpp"
#include "UserInterface.hpp"
#include "CharacterList.hpp"

#include "..\R3E\Project.hpp"
#include "..\R3E\TargetCamera.hpp"
#include "..\R3E\SceneManager.hpp"

InGameState::InGameState(){
}

InGameState::~InGameState(){
}

bool InGameState::CanEnterState(){
	return true;
}

void InGameState::EnterState(){
	ChatDialog* dlg = (ChatDialog*)gInterface->FindDialog(DLG_CHAT_BOX);
	if(dlg) dlg->SetVisible(true);

	gPlayer->SetPosition(gPlayer->GetPosition());//fix z pos
	gScene->AddEntity(gPlayer);

	mCamera = new TargetCamera();
	mCamera->SetDistance(10.0f);
	mCamera->FollowEntity(gPlayer);
	gScene->SetCamera(mCamera);

	Packet pakout(0x753);
	pakout.AddVal<char>(1);//weight rate
	pakout.AddVal<short>(0);//zpos
	gNetwork->SendPacket(GS, &pakout);
}

bool InGameState::LeaveState(){
	gScene->SetCamera(NULL);
	delete mCamera;

	mActive = false;
	return true;
}

Vector3 mMousePos;

int InGameState::HandleEvent(GuiEvent* gevt){
	if(gevt->_evt_type == EVT_MOUSE){
		MouseEvent* evt = (MouseEvent*)gevt;
		if(evt->type == MOUSE_MOVE){
			static Point last;

			if(evt->button & RBUTTON){
				int dX = last.x - evt->abspos.x;
				mCamera->RotateH(float(dX) / 100.0f);

				int dY = last.y - evt->abspos.y;
				mCamera->RotateV(float(dY) / 100.0f);
			}

			last = evt->abspos;
		}
	}

	if(int id = gInterface->HandleEvent(gevt))
		return id;

	if(gevt->_evt_type == EVT_MOUSE){
		MouseEvent* evt = (MouseEvent*)gevt;
		if(evt->type == WHEEL)
			mCamera->AddDistance(float(evt->button) / 40.0f);
		else if(evt->type == BUTTON_UP && evt->button == LBUTTON){
			Vector2 wnd = Vector2(float(gWindow->Width()), float(gWindow->Height()));
			Vector3 mouse = Vector3(float(evt->pos.x), float(gWindow->Height() - evt->pos.y), 0.0f);
			Ray ray = UnprojectRay(mouse, mCamera->GetTransform(), mCamera->GetProjection(), wnd);

			Vector3 pos;
			if(gScene->CastRay(ray, pos, 1200000.0f)){
				Packet pakout(0x79A);
				pakout.AddVal<short>(0);
				pakout.AddVal<float>(pos.x * 100.0f);
				pakout.AddVal<float>(pos.y * 100.0f);
				pakout.AddVal<short>(short(pos.z * 100.0f));
				gNetwork->SendPacket(GS, &pakout);
				mMousePos = pos;
			}
		}
	}

	return 0;
}


int InGameState::HandleEvent(GameEvent* /*gevt*/){
	return 0;
}

void InGameState::DeleteObject(unsigned int cid){
	Character* chr = (Character*)gCharacterList->Get(cid);
	if(chr){
		switch(chr->GetCharacterType()){
			case CHR_AVATAR:
				{
					Avatar* avatar = (Avatar*)chr;
					gScene->RemoveEntity(avatar);
					gCharacterList->Remove(chr);
					delete avatar;
				}
				break;
			case CHR_NPC:
				{
					Npc* npc = (Npc*)chr;
					gScene->RemoveEntity(npc);
					gCharacterList->Remove(chr);
					delete npc;
				}
				break;
			case CHR_MONSTER:
				{
					Monster* mon = (Monster*)chr;
					gScene->RemoveEntity(mon);
					gCharacterList->Remove(chr);
					delete mon;
				}
				break;
			default:
				printf("Character with CID:%X invalid type!\n", cid);
		};
	}else
		printf("Character with CID:%X not found!\n", cid);
}

int InGameState::HandleEvent(NetworkEvent* nevt){
	switch(nevt->_evt_type){
		case NET_PAK_MOVE_PLAYER:
		case NET_PAK_MOVE_NPC:
		{
			PakNetMovePlayerEvent* evt = (PakNetMovePlayerEvent*)nevt;
			Character* chr = gCharacterList->Get(evt->mClientID);
			if(chr)
				chr->MoveTo(evt->mTargetPosition);
			else
				printf("Character with CID:%X not found!\n", evt->mClientID);
			
			//if NET_PAK_MOVE_NPC then mMoveMode for run/walk
		}
		break;
		case NET_PAK_JOIN_ZONE:
		{
			PakNetJoinZone* evt = (PakNetJoinZone*)nevt;
			gPlayer->SetClientID(evt->mClientID);
			gCharacterList->Add(gPlayer);
		}
		break;
		case NET_PAK_SPAWN_NPC:
		{
			PakSpawnNpcEvent* evt = (PakSpawnNpcEvent*)nevt;
			
			Npc* npc = new Npc();
			npc->SetNpcID(evt->mData.mCharacterIndex);
			npc->SetClientID(evt->mData.mClientID);
			npc->SetPosition(evt->mData.mPosition);
			npc->SetMoveSpeed(500);

			if(evt->mData.mCommand == CMD_RUNAWAY || evt->mData.mCommand == CMD_MOVE || evt->mData.mCommand == CMD_PICK_ITEM)
				npc->MoveTo(evt->mData.mTargetPosition);

			gScene->AddEntity(npc);
			gCharacterList->Add(npc);
		}
		break;
		case NET_PAK_SPAWN_MONSTER:
		{
			PakSpawnMonsterEvent* evt = (PakSpawnMonsterEvent*)nevt;
			
			Monster* mon = new Monster();
			mon->SetNpcID(evt->mData.mCharacterIndex);
			mon->SetClientID(evt->mData.mClientID);
			mon->SetPosition(evt->mData.mPosition);
			mon->SetMoveSpeed(500);

			if(evt->mData.mCommand == CMD_RUNAWAY || evt->mData.mCommand == CMD_MOVE || evt->mData.mCommand == CMD_PICK_ITEM)
				mon->MoveTo(evt->mData.mTargetPosition);

			gScene->AddEntity(mon);
			gCharacterList->Add(mon);
		}
		break;
		case NET_PAK_SPAWN_AVATAR:
		{
			PakSpawnAvatarEvent* evt = (PakSpawnAvatarEvent*)nevt;

			Avatar* avatar = new Avatar();
			avatar->SetName(evt->mName);
			avatar->SetGender(evt->mData.mGender);
			avatar->SetItems(evt->mData.mEquipment);
			avatar->SetClientID(evt->mData.mClientID);
			avatar->SetPosition(evt->mData.mPosition);
			avatar->SetMoveSpeed(evt->mData.mMoveSpeed);
			avatar->SetFaceStyle(evt->mData.mEquipment[BP_FACE].mItemNo);
			avatar->SetHairStyle(evt->mData.mEquipment[BP_HAIR].mItemNo);

			if(evt->mData.mCommand == CMD_RUNAWAY || evt->mData.mCommand == CMD_MOVE || evt->mData.mCommand == CMD_PICK_ITEM)
				avatar->MoveTo(evt->mData.mTargetPosition);

			gScene->AddEntity(avatar);
			gCharacterList->Add(avatar);
		}
		break;
		case NET_PAK_DELETE_OBJECT:
		{
			PakDeleteObjectEvent* evt = (PakDeleteObjectEvent*)nevt;
			for(unsigned int i = 0; i < evt->mCount; ++i)
				DeleteObject(evt->mClientIDs[i]);
		}
		break;
		case NET_PAK_TELEPORT:
		{
			PakNetTeleportEvent* evt = (PakNetTeleportEvent*)nevt;
			gPlayer->SetPosition(evt->mPosition);
			gStateManager->EnqueueState(new LoadState(evt->mZone));
			gStateManager->EnqueueState(new InGameState());
			LeaveState();
		}
		break;
		case NET_PAK_LOCAL_CHAT:
		{
			PakNetLocalChat* evt = (PakNetLocalChat*)nevt;
			Character* chr = gCharacterList->Get(evt->mClientID);
			if(chr){
				char buffer[256];
				int nlen = strlen(chr->GetName());
				int mlen = strlen(evt->mMessage);
				if(nlen + mlen >= 254) mlen = 254 - nlen;

				memcpy(buffer, chr->GetName(), nlen);
				buffer[nlen] = '>';
				memcpy(buffer + nlen + 1, evt->mMessage, mlen);
				buffer[nlen + mlen + 1] = 0;
				
				ChatDialog* dlg = (ChatDialog*)gInterface->FindDialog(DLG_CHAT_BOX);
				dlg->AddChatMessage(buffer, CHAT_TYPE_ALL);
			}
		}
		break;
		case NET_PAK_DAMAGE:
		{
			PakNetDamage* evt = (PakNetDamage*)nevt;
			if(evt->mValue & DMG_BIT_DEAD)
				DeleteObject(evt->mDefenderCID);
		}
		break;
	};

	return 0;
}

int InGameState::Run(){
	mActive = true;

	while(mActive){
		gScene->BeginScene();
		gScene->RenderScene();
		glBegin(GL_LINES);
		glVertex3f(mMousePos.x, mMousePos.y, mMousePos.z);
		glVertex3f(mMousePos.x, mMousePos.y, mMousePos.z + 1.0f);
		glEnd();
		gScene->EndScene();
		
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);

		gScene->Begin2D();
		gInterface->Render();
		OpenGL::DepthTest(true);
		OpenGL::DepthWrite(true);
		gCharacterList->Draw2D();
		gScene->End2D();

		gScene->UpdateScene();

		gNetwork->Process();
		gGameEvents->Process();

		gWindow->SwapBuffers();
	}

	return 0;
}
