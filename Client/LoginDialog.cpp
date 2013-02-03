#include "Global.h"

#include "Window.hpp"
#include "DialogType.hpp"
#include "GameEvents.hpp"
#include "LoginDialog.hpp"
#include "..\RGE\Button.hpp"
#include "..\RGE\EditBox.hpp"

LoginDialog::LoginDialog(){
	SetID(DLG_LOGIN);
}

LoginDialog::~LoginDialog(){
}

LoginDialog* LoginDialog::Create(){
	LoginDialog* dlg = new LoginDialog();
	dlg->CreateFromXML("3DDATA\\CONTROL\\XML\\DlgLogin.xml");
	return dlg;
}

void LoginDialog::OnCreated(){
	SetPositionX(gWindow->Width() - SizeX() - 50);
	SetPositionY(100);

	mUsername = (EditBox*)GetControlByID(2);
	mPassword = (EditBox*)GetControlByID(3);

	mButtonOK = (Button*)GetControlByID(4);
	mButtonExit = (Button*)GetControlByID(5);
}

int LoginDialog::HandleEvent(MouseEvent* evt){
	int id = Dialog::HandleEvent(evt);
	if(evt->type == BUTTON_UP){
		if(id == mButtonOK->ID()){
			EmitLoginEvent();
		}else if(id == mButtonExit->ID()){
			QuitEvent* evt = gGameEvents->CreateEvent<QuitEvent>();
			gGameEvents->Emit(evt);
		}
	}

	return id;
}

int LoginDialog::HandleEvent(KeyboardEvent* evt){
	int id = Dialog::HandleEvent(evt);
	if(!id) return 0;
	if(evt->type == KEY_UP){
		if(evt->key == VK_TAB){
			if(id == mUsername->ID())
				mPassword->SetFocus(true);
			else if(id == mPassword->ID())
				mUsername->SetFocus(true);
		}else if(evt->key == VK_RETURN)
			EmitLoginEvent();
	}

	return id;
}

void LoginDialog::EmitLoginEvent(){
	LoginEvent* evt = gGameEvents->CreateEvent<LoginEvent>();
	evt->mUsername = mUsername->GetText();
	evt->mPassword = mPassword->GetText();
	gGameEvents->Emit(evt);
}
