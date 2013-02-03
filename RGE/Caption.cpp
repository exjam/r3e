#include "Global.h"

#include "Caption.hpp"

Caption::Caption() : mClicked(false) {
	mControlType = CT_CAPTION;
}

Caption::~Caption(){}

void Caption::Render(){
}

int Caption::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	if(evt->button != LBUTTON) return 0;
	if(!IsInside(evt->pos)) return 0;

	if(evt->type == BUTTON_DOWN)
		mClicked = true;
	else if(evt->type == BUTTON_UP)
		mClicked = false;

	return 1;
}

int Caption::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int Caption::Update(Point mouse){
	return Visible() && IsInside(mouse);
}

bool Caption::IsClicked(){
	return mClicked;
}
