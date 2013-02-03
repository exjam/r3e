#include "Global.h"

#include "ListBox.hpp"
#include "Windows.hpp"

ListBox::ListBox()
	: mAutoResize(false), mCharHeight(0), mLineSpace(0), mFitLines(0), mSelected(0), mSelectable(0), mFont(0), mMaxSize(0), mOwnerDraw(0), mLineIdx(0)
{
	mControlType = CT_LISTBOX;
}

ListBox::~ListBox(){
	mItems.Clear();
}

void ListBox::Render(){
	if(!Visible() || !mItems.Size()) return;

	int transX = mPosition.x;
	int transY = mPosition.y;
	int incY = mCharHeight + mLineSpace;

	int start = mLineIdx;
	unsigned int end = start + mFitLines;
	unsigned int max = mItems.Size();
	if(end > max){
		end = mItems.Size();
		start = end - mFitLines;
		if(start < 0) start = 0;
		mLineIdx = start;
	}

	gUiRender->PushTranslate(transX, transY);
	for(unsigned int i = start; i < end; ++i){
		ListItem* item = &mItems[i];

		if(mSelected == i && mSelectable)
			gUiRender->SetDrawColour(255, 255, 128);
		else
			gUiRender->SetDrawColour(&item->mColour);

		gUiRender->DrawText(item->mText);
		gUiRender->PushTranslate(0, incY);
		transY += incY;
	}

	gUiRender->PopTranslate(transX, transY);
	gUiRender->ResetDrawColour();

	return;
}

int ListBox::HandleEvent(MouseEvent* evt){
	if(evt->type != BUTTON_UP) return 0;
	if(evt->button != LBUTTON) return 0;
	if(!IsInside(evt->pos)) return 0;
	int y = evt->pos.y - mPosition.y;
	y /= (mCharHeight + mLineSpace);
	if(y < 0) return 0;
	if(unsigned int(y) >= mItems.Size()) return 0;
	mSelected = y;
	return mID;
}

int ListBox::HandleEvent(KeyboardEvent* evt){
	if(evt->type == KEY_CHAR){
		if(evt->key == VK_UP){
			if(mSelected > 0)
				--mSelected;
		}else if(evt->key == VK_DOWN){
			++mSelected;
			if(mSelected >= mItems.Size())
				mSelected = mItems.Size() - 1;
		}
	}

	return 0;
}

int ListBox::Update(Point mouse){
	return Visible() && IsInside(mouse);
}

int ListBox::AddItem(const char* text, Colour colour){
	if(mCharHeight + mLineSpace == 0)
		mLineSpace = 10;

	mFitLines = (SizeY() / (mCharHeight + mLineSpace)) + 1;
	if(mItems.Size() == 0 && mSize.y == 0)
		mAutoResize = true;

	ListItem* item = &mItems.Add();
	item->mValue = text;
	item->mText = gUiRender->CreateTextWithFont(mFont);
	item->mText->SetText(text);
	item->mColour = colour;

	if(mAutoResize)
		mSize.y = mItems.Size() * (mCharHeight + mLineSpace);

	return mItems.Size() - 1;
}

void ListBox::Clear(){
	mItems.Clear();
}

void ListBox::SetItem(int idx, const char* text){
	ListItem* item = &mItems[idx];
	item->mValue = text;
	item->mText->SetText(text);
}

void ListBox::DeleteItem(int idx){
	mItems.Erase(idx);
}

const char* ListBox::GetItem(int idx){
	return mItems[idx].mValue;
}

unsigned int ListBox::ItemCount(){
	return mItems.Size();
}
