#include "Global.h"

#include "Dialog.hpp"

Dialog::Dialog()
	: mModal(0), mCaption(0), mShowSoundID(0), mHideSoundID(0), mExtent(0), mDefaultVisible(0)
{
	mControlType = CT_DIALOG;	
}

Dialog::~Dialog(){
	mChildren.DeleteValues();
	SAFE_DELETE(mCaption);
}

void Dialog::OnCreated(){}

void Dialog::AddChild(IControl* child){
	if(!child) return;
	mChildren.PushBack(child);
}

void Dialog::Render(){
	if(!Visible()) return;

	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mChildren[i]->Render();

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int Dialog::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	if(!IsInside(evt->pos)) return 0;

	evt->pos -= mPosition;
	Point pos = evt->pos;

	int ret = 1;
	if(mCaption){
		mCaption->HandleEvent(evt);

		if(evt->type == BUTTON_DOWN && evt->button == LBUTTON && mCaption->IsClicked()){
			mCaptionClicked = pos;
			ret = 1;
		}
	}

	for(unsigned int i = 0; i < mChildren.Size(); ++i){
		if(int id = mChildren[i]->HandleEvent(evt))
			ret = id;

		evt->pos = pos;
	}

	return ret;
}

int Dialog::HandleEvent(KeyboardEvent* evt){
	if(!Visible()) return 0;

	int ret = 0;

	for(unsigned int i = 0; i < mChildren.Size(); ++i){
		if(int id = mChildren[i]->HandleEvent(evt))
			ret = id;
	}

	return ret;
}

int Dialog::Update(Point mouse){
	if(!Visible()) return 0;

	int ret = 0;

	mouse -= mPosition;

	if(mCaption && mCaption->IsClicked()){
		Point move = mouse - mCaptionClicked;
		mPosition += move;
	}

	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		ret |= mChildren[i]->Update(mouse);

	return ret;
}

IControl* Dialog::GetControlByID(int id){
	IControl* ctrl;

	for(unsigned int i = 0; i < mChildren.Size(); ++i){
		if(ctrl = mChildren[i]->GetControlByID(id))
			return ctrl;
	}

	return NULL;
}
