#include "Global.h"

#include "TabbedPane.hpp"

TabbedPane::TabbedPane() : mActiveTab(0) {
	mControlType = CT_TABBEDPANE;
}

TabbedPane::~TabbedPane(){
	mTabs.DeleteValues();
}

void TabbedPane::AddTab(Tab* child){
	mTabs.PushBack(child);
	mRadioBox.AddButton(child->TabButton());
	child->SetVisible(false);
}

void TabbedPane::Render(){
	if(!Visible() || mTabs.Size() == 0) return;

	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	for(unsigned int i = 0; i < mTabs.Size(); ++i)
		mTabs[i]->Render();

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int TabbedPane::HandleEvent(MouseEvent* evt){
	if(!Visible() || mTabs.Size() == 0) return 0;

	int ret = 0;

	evt->pos -= mPosition;
	Point pos = evt->pos;

	for(unsigned int i = 0; i < mTabs.Size(); ++i){
		if(int id = mTabs[i]->HandleEvent(evt)){
			ret = id;
			
			if(evt->button == LBUTTON && evt->type == BUTTON_UP)
				SetActiveTab(i);
		}

		evt->pos = pos;
	}

	return ret;
}

int TabbedPane::HandleEvent(KeyboardEvent* evt){
	if(!Visible() || mTabs.Size() == 0) return 0;

	int ret = 0;

	for(unsigned int i = 0; i < mTabs.Size(); ++i)
		if(int id = mTabs[i]->HandleEvent(evt))
			ret = id;

	return ret;
}

int TabbedPane::Update(Point mouse){
	if(!Visible() || mTabs.Size() == 0) return 0;

	int ret = 0;
	mouse -= mPosition;

	for(unsigned int i = 0; i < mTabs.Size(); ++i)
		ret |= mTabs[i]->Update(mouse);

	if(mActiveTab != mRadioBox.SelectedIdx())
		SetActiveTab(mRadioBox.SelectedIdx());

	return ret;
}

unsigned int TabbedPane::ActiveTab(){
	return mActiveTab;
}

void TabbedPane::SetActiveTab(unsigned int tab){
	if(tab >= mTabs.Size()) return;
	mActiveTab = tab;
	for(unsigned int i = 0; i < mTabs.Size(); ++i)
		mTabs[i]->SetVisible(false);

	mTabs[tab]->SetVisible(true);
	mRadioBox.SetSelectedIdx(tab);
}

IControl* TabbedPane::GetControlByID(int id){
	IControl* ctrl;
	if(ctrl = IControl::GetControlByID(id))
		return ctrl;

	for(unsigned int i = 0; i < mTabs.Size(); ++i){
		if(ctrl = mTabs[i]->GetControlByID(id))
			return ctrl;
	}

	return NULL;
}