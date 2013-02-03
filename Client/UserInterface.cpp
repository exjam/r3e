#include "Global.h"

#include "UserInterface.hpp"

UserInterface::UserInterface() : mMsgBox(0) {}
UserInterface::~UserInterface(){
	mDialogs.DeleteValues();
	SAFE_DELETE(mMsgBox);
}

void UserInterface::Init(){
	gUiRender = new UiRender();
	ImageRes::Instance().Load();
	CreateDialogs();
}

void UserInterface::AddDialog(Dialog* dlg){
	mDialogs.PushBack(dlg);
	dlg->SetVisible(false);
}

void UserInterface::RemoveDialog(Dialog* dlg){
	int idx = mDialogs.Find(dlg);
	if(idx == Array<Dialog*, 5>::NOT_FOUND) return;
	mDialogs.Erase(idx);
}

Dialog* UserInterface::FindDialog(int id){
	for(unsigned int i = 0; i < mDialogs.Size(); ++i)
		if(mDialogs[i]->ID() == id)
			return mDialogs[i];
	
	return NULL;
}

void UserInterface::Render(){
	Update();

	for(unsigned int i = 0; i < mDialogs.Size(); ++i)
		mDialogs[i]->Render();

	if(UiState::mModal)
		return UiState::mModal->Render();
}

int UserInterface::Update(){
	if(UiState::mModal)
		return UiState::mModal->Update(mMousePos);

	for(int i = int(mDialogs.Size()) - 1; i >= 0; --i){
		if(mDialogs[i]->Update(mMousePos))
			return 1;
	}

	return 0;
}

int UserInterface::HandleEvent(UINT uiMsg, WPARAM wParam, LPARAM lParam){
	return HandleEvent(TranslateWin32Event(uiMsg, wParam, lParam));
}

int UserInterface::HandleEvent(GuiEvent* gevt){
	if(!gevt) return 0;

	if(gevt->_evt_type == EVT_MOUSE){
		MouseEvent* evt = (MouseEvent*)gevt;

		if(evt->type == MOUSE_MOVE){
			mMousePos = evt->abspos;
			return 0;
		}

		if(UiState::mModal)
			return UiState::mModal->HandleEvent(evt);

		for(int i = int(mDialogs.Size()) - 1; i >= 0; --i){
			if(!mDialogs[i]->HandleEvent(evt))
				continue;

			if(evt->type == BUTTON_DOWN && evt->button == LBUTTON){
				Dialog* dlg = mDialogs[i];
				mDialogs.Erase(i);
				mDialogs.PushBack(dlg);
			}

			return 1;
		}

		if(evt->type == BUTTON_UP && evt->button == LBUTTON)
			UiState::mFocus = NULL;
	}else if(gevt->_evt_type == EVT_KEYBOARD){
		KeyboardEvent* evt = (KeyboardEvent*)gevt;

		if(UiState::mModal)
			return UiState::mModal->HandleEvent(evt);

		for(int i = int(mDialogs.Size()) - 1; i >= 0; --i){
			if(mDialogs[i]->HandleEvent(evt))
				return 1;
		}
	}

	return 0;
}

void UserInterface::ShowMsgBox(const char* msg, int buttons){
	if(!mMsgBox)
		mMsgBox = MsgBox::Create();
	
	UiState::mModal = mMsgBox;
	mMsgBox->SetMessage(msg);
	mMsgBox->SetButtons(buttons);
	mMsgBox->SetVisible(true);

	int x = (gWindow->Width() - mMsgBox->SizeX()) / 2;
	int y = (gWindow->Height() - mMsgBox->SizeY()) / 2;

	mMsgBox->SetPosition(Point(x, y));
}

void UserInterface::HideMsgBox(){
	if(!mMsgBox) return;
	mMsgBox->SetVisible(false);
	UiState::mModal = NULL;
}

#include "ChatDialog.hpp"
#include "LoginDialog.hpp"
#include "ServerListDialog.hpp"
#include "CharacterSelectDialog.hpp"

void UserInterface::CreateDialogs(){
	gInterface->AddDialog(ChatDialog::Create());
	gInterface->AddDialog(LoginDialog::Create());
	gInterface->AddDialog(ServerListDialog::Create());
	gInterface->AddDialog(CharacterSelectDialog::Create());
}
