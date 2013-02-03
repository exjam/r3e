#include "Global.h"

#include "Window.hpp"
#include "DialogType.hpp"
#include "GameEvents.hpp"
#include "ServerListDialog.hpp"

#include "..\RGE\Button.hpp"
#include "..\RGE\ListBox.hpp"
#include "..\RNE\PacketEvents.hpp"

ServerListDialog::ServerListDialog(){
	SetID(DLG_SERVER_LIST);
}

ServerListDialog::~ServerListDialog(){
}

ServerListDialog* ServerListDialog::Create(){
	ServerListDialog* dlg = new ServerListDialog();
	dlg->CreateFromXML("3DDATA\\CONTROL\\XML\\DlgSelSvr.xml");
	return dlg;
}

void ServerListDialog::OnCreated(){
	SetPositionX(gWindow->Width() - SizeX() - 30);
	SetPositionY(100);

	mServerList = (ListBox*)GetControlByID(2);
	mChannelList = (ListBox*)GetControlByID(3);

	mButtonOK = (Button*)GetControlByID(10);
	mButtonCancel = (Button*)GetControlByID(11);
}

int ServerListDialog::HandleEvent(MouseEvent* evt){
	int id = Dialog::HandleEvent(evt);
	if(evt->type == BUTTON_UP){
		if(id == mButtonOK->ID()){
			SelectServerEvent* evt = gGameEvents->CreateEvent<SelectServerEvent>();
			evt->mServer = mServers[mServerList->Selected()];
			evt->mChannel = mChannels[mChannelList->Selected()];
			gGameEvents->Emit(evt);
		}else if(id == mButtonCancel->ID()){
			LogoutEvent* evt = gGameEvents->CreateEvent<LogoutEvent>();
			gGameEvents->Emit(evt);
		}
	}

	return id;
}

void ServerListDialog::AddServer(WorldServer* svr){
	mServerList->AddItem(svr->mName.Str() + 1);
	mServers.PushBack(svr->mID);
}

void ServerListDialog::ClearServers(){
	mServerList->Clear();
}

void ServerListDialog::ClearChannels(){
	mChannelList->Clear();
}

void ServerListDialog::AddChannel(int /*server*/, GameChannel* channel){
	mChannelList->AddItem(channel->mName);
	mChannels.PushBack(channel->mID);
}
