#include "Global.h"

#include "RadioBox.hpp"

RadioBox::RadioBox() : mSelectedIdx(0) {
	mControlType = CT_RADIOBOX;
}

RadioBox::~RadioBox(){
	mChildren.Clear();
}

void RadioBox::Render(){
}

int RadioBox::HandleEvent(MouseEvent* /*evt*/){
	return 0;
}

int RadioBox::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int RadioBox::Update(Point /*mouse*/){
	return 0;
}

void RadioBox::AddButton(RadioButton* btn){
	mChildren.PushBack(btn);

	btn->SetParentBox(this);
}

void RadioBox::RemoveButton(RadioButton* btn){
	mChildren.Erase(mChildren.Find(btn));
}

void RadioBox::SetSelected(RadioButton* opt){
	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		if(mChildren[i] != opt) mChildren[i]->SetButtonState(0);
		else mSelectedIdx = i;
}
