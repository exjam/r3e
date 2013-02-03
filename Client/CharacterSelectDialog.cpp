#include "Global.h"

#include "Window.hpp"
#include "DialogType.hpp"
#include "GameEvents.hpp"
#include "CharacterSelectDialog.hpp"

#include "..\RGE\Button.hpp"

CharacterSelectDialog::CharacterSelectDialog(){
	SetID(DLG_CHARACTER_SELECT);
}

CharacterSelectDialog::~CharacterSelectDialog(){
}

CharacterSelectDialog* CharacterSelectDialog::Create(){
	CharacterSelectDialog* dlg = new CharacterSelectDialog();
	dlg->CreateFromXML("3DDATA\\CONTROL\\XML\\DlgSelAvatar.xml");
	return dlg;
}

void CharacterSelectDialog::OnCreated(){
	mButtonCreate = (Button*)GetControlByID(10);
	mButtonDelete = (Button*)GetControlByID(11);
	mButtonStart = (Button*)GetControlByID(12);
	mButtonExit = (Button*)GetControlByID(13);

	int width = 0;
	mButtonCreate->SetPositionX(width);
	mButtonCreate->SetPositionY(0);
	width += mButtonCreate->SizeX();

	mButtonDelete->SetPositionX(width);
	mButtonDelete->SetPositionY(0);
	width += mButtonDelete->SizeX();

	mButtonExit->SetPositionX(width);
	mButtonExit->SetPositionY(0);
	width += mButtonExit->SizeX();

	mButtonStart->SetPositionX(width);
	mButtonStart->SetPositionY(0);
	width += mButtonStart->SizeX();

	int maxBtnHeight = mButtonExit->SizeY();
	if(mButtonCreate->SizeY() < maxBtnHeight) maxBtnHeight = mButtonCreate->SizeY();
	if(mButtonDelete->SizeY() < maxBtnHeight) maxBtnHeight = mButtonDelete->SizeY();
	if(mButtonStart->SizeY() < maxBtnHeight)  maxBtnHeight = mButtonStart->SizeY();

	int xpos = (gWindow->Width() / 2) - (width / 2);
	int ypos = gWindow->Height() - 24 - maxBtnHeight;

	SetPosition(Point(xpos, ypos));
	SetSize(Point(width, maxBtnHeight));
}

int CharacterSelectDialog::HandleEvent(MouseEvent* evt){
	int id = Dialog::HandleEvent(evt);
	if(evt->type == BUTTON_UP){
		if(id == mButtonCreate->ID()){
		}else if(id == mButtonDelete->ID()){
		}else if(id == mButtonStart->ID()){
			SelectCharacterEvent* evt = gGameEvents->CreateEvent<SelectCharacterEvent>();
			evt->mCharacterIdx = -1;
			gGameEvents->Emit(evt);
		}else if(id == mButtonExit->ID()){
		}
	}

	return id;
}
