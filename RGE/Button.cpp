#include "Global.h"

#include "Button.hpp"

Button::Button() : mEnabled(true), mButtonState(BS_NORMAL) {
	mControlType = CT_BUTTON;
}

Button::~Button(){}

void Button::Render(){
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

int Button::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	if(evt->button != LBUTTON) return 0;
	if(!IsInside(evt->pos)) return 0;
	if(!Enabled()) return mID;

	if(evt->type == BUTTON_DOWN)
		mButtonState = BS_CLICKED;
	else if(evt->type == BUTTON_UP && mButtonState == BS_CLICKED)
		mButtonState = BS_MOUSEOVER;

	return mID;
}

int Button::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int Button::Update(Point mouse){
	if(!Visible()) return 0;

	if(!IsInside(mouse)){
		mButtonState = BS_NORMAL;

		return 0;
	}else{
		if(mButtonState != BS_CLICKED)
			mButtonState = BS_MOUSEOVER;

		return 1;
	}
}
