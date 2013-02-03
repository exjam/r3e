#include "Global.h"

#include "Tab.hpp"

Tab::Tab(TabbedPane* parent) : mParent(parent), mTabButton(0) {
	mControlType = CT_TAB;
}

Tab::~Tab(){
	mChildren.DeleteValues();
}

void Tab::AddChild(IControl* child){
	if(!child) return;
	mChildren.PushBack(child);
}

void Tab::Render(){
	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	if(mTabButton)
		mTabButton->Render();

	if(Visible()){
		for(unsigned int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->Render();
	}

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int Tab::HandleEvent(MouseEvent* evt){
	if(mTabButton && mTabButton->HandleEvent(evt))
		return mTabButton->ID();

	if(!Visible()) return 0;

	int ret = 0;
	evt->pos -= mPosition;
	Point pos = evt->pos;

	for(unsigned int i = 0; i < mChildren.Size(); ++i){
		if(int id = mChildren[i]->HandleEvent(evt))
			ret = id;

		evt->pos = pos;
	}

	return 0;
}

int Tab::HandleEvent(KeyboardEvent* evt){
	if(!Visible()) return 0;

	int ret = 0;

	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		if(int id = mChildren[i]->HandleEvent(evt))
			ret = id;

	return ret;
}

int Tab::Update(Point mouse){
	int ret = 0;
	mouse -= mPosition;

	if(mTabButton)
		ret |= mTabButton->Update(mouse);

	if(Visible()){
		for(unsigned int i = 0; i < mChildren.Size(); ++i)
			ret |= mChildren[i]->Update(mouse);
	}

	return ret;
}

IControl* Tab::GetControlByID(int id){
	IControl* ctrl;
	if(ctrl = IControl::GetControlByID(id))
		return ctrl;

	for(unsigned int i = 0; i < mChildren.Size(); ++i){
		if(ctrl = mChildren[i]->GetControlByID(id))
			return ctrl;
	}

	return NULL;
}
