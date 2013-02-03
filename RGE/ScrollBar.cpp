#include "Global.h"

#include "ScrollBar.hpp"
#include "ScrollBox.hpp"
#include "IScrollModel.hpp"

ScrollBar::ScrollBar()
	: mListBoxID(0), mType(SBT_VERTICAL), mModel(0), mScrollBox(0)
{
}

ScrollBar::~ScrollBar(){
	SAFE_DELETE(mScrollBox);
}

void ScrollBar::Render(){
	if(!Visible()) return;
	gUiRender->PushTranslate(mPosition.x, mPosition.y);
	mScrollBox->Render();
	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int ScrollBar::Update(Point mouse){
	if(!Visible()) return 0;
	mouse -= mPosition;
	int ret = IsInside(mouse);
	if(mScrollBox)
		ret |= mScrollBox->Update(mouse);

	return ret;
}

int ScrollBar::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	evt->pos -= mPosition;
	int ret = 0;
	if(mScrollBox)
		ret |= mScrollBox->HandleEvent(evt);

	return ret;
}

int ScrollBar::HandleEvent(KeyboardEvent* evt){
	if(!Visible()) return 0;
	int ret = 0;
	if(mScrollBox)
		ret |= mScrollBox->HandleEvent(evt);

	return ret;
}

void ScrollBar::SetModel(IScrollModel* model){
	mModel = model;
}

void ScrollBar::SetScrollBox(ScrollBox* box){
	mScrollBox = box;
}

void ScrollBar::SetScrollPercent(float percent){
	if(!mModel) return;
	int max = mModel->GetMax();
	int min = mModel->GetMin();
	int d = max - min;
	mModel->SetValue(int(float(d) * percent));
}

void ScrollBar::ScrollToTop(){
	mScrollBox->ScrollToTop();
}

void ScrollBar::ScrollToBottom(){
	mScrollBox->ScrollToBottom();
}
