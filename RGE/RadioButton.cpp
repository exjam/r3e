#include "Global.h"

#include "RadioButton.hpp"
#include "RadioBox.hpp"

RadioButton::RadioButton() : mButtonState(BS_NORMAL), mParentBox(0) {
	mControlType = CT_RADIOBUTTON;
}

RadioButton::~RadioButton(){}

void RadioButton::Render(){
	if(!Visible()) return;
	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	if(mButtonState == BS_NORMAL && mNormalGraphic)
		mNormalGraphic->Render();
	else if(mButtonState == BS_MOUSEOVER && mOverGraphic)
		mOverGraphic->Render();
	else if(mButtonState == BS_CLICKED && mDownGraphic)
		mDownGraphic->Render();

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int RadioButton::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	if(evt->button != LBUTTON) return 0;
	if(evt->type != BUTTON_DOWN) return 0;
	if(!IsInside(evt->pos)) return 0;

	mButtonState = (mButtonState == BS_CLICKED)?BS_MOUSEOVER:BS_CLICKED;
	if(mParentBox) mParentBox->SetSelected(this);

	return mID;
}

int RadioButton::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int RadioButton::Update(Point mouse){
	if(!Visible()) return 0;

	if(!IsInside(mouse)){
		if(mButtonState == BS_MOUSEOVER)
			mButtonState = BS_NORMAL;

		return 0;
	}else{
		if(mButtonState != BS_CLICKED)
			mButtonState = BS_MOUSEOVER;

		return 1;
	}
}
