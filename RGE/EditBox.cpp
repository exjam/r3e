#include "Global.h"

#include "EditBox.hpp"
#define NOGDI
#include "Windows.hpp"

EditBox::EditBox()
	: mClicked(false), mText(0), mPassword(0), mSelectStart(0), mSelectEnd(0),
	mCaretVisible(true), mCaretTimer(clock()), mFont(0), mTextColour(0)
{
	mControlType = CT_EDITBOX;
	mString[0] = 0;
}

EditBox::~EditBox(){
	if(mText)
		gUiRender->FreeText(mText);
}

void EditBox::Render(){
	if(!Visible() || !mText) return;

	gUiRender->PushTranslate(mPosition.x, mPosition.y);
	gUiRender->DrawText(mText);

	if(UiState::mFocus == this){
		if(mSelectStart != mSelectEnd){
			int x1, x2, h = GetTextHeight();
			x1 = GetXFromCaret(mSelectStart);
			x2 = GetXFromCaret(mSelectEnd);

			gUiRender->SetDrawColour(0, 150, 255, 128);
			gUiRender->DrawRect(x1, 0, x2, h);
			gUiRender->ResetDrawColour();
		}

		if(clock() - mCaretTimer > 600){
			mCaretVisible = !mCaretVisible;
			mCaretTimer = clock();
		}

		if(mCaretVisible){
			int y = mText->GetCharHeight('y') - 1;
			int x1 = GetXFromCaret(mCaret);
			gUiRender->DrawLine(x1, 0, x1, y);
		}
	}

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int EditBox::Update(Point mouse){
	if(!Visible()) return 0;

	if(mClicked){
		mSelectStart = GetCaretFromX(mClickPos.x - mPosition.x);
		mSelectEnd = GetCaretFromX(mouse.x - mPosition.x);
		mCaret = mSelectEnd;
		AlignSelection();
	}

	return IsInside(mouse) || mClicked;
}

void EditBox::SetFocus(bool focus){
	if(focus){
		UiState::mFocus = this;
		mCaret = 0;
		mSelectStart = 0;
		mSelectEnd = 0;
	}else if(!focus){
		if(UiState::mFocus == this)
			UiState::mFocus = NULL;
	}
}

int EditBox::HandleEvent(MouseEvent* evt){
	if(!Visible()) return 0;
	if(evt->button != LBUTTON) return 0;

	bool inside = IsInside(evt->pos);
	if(evt->type == BUTTON_DOWN && inside){
		mClicked = true;
		mClickPos = evt->pos;
		UiState::mFocus = this;
	}else if(evt->type == BUTTON_UP){
		mClicked = false;
		if(!inside) return 0;
		int cStart = GetCaretFromX(mClickPos.x - mPosition.x);
		int cEnd = GetCaretFromX(evt->pos.x - mPosition.x);
		mCaret = cEnd;

		if(cStart == cEnd){
			mSelectStart = mCaret;
			mSelectEnd = mCaret;
		}
	}

	return mID;
}

int EditBox::HandleEvent(KeyboardEvent* evt){
	if(!Visible()) return 0;
	if(UiState::mFocus != this) return 0;

	if(evt->type == KEY_DOWN)
		OnKeyDown(evt->key);
	else if(evt->type == KEY_CHAR)
		OnChar(evt->key);

	return mID;
}

void EditBox::SetTextElement(IText* text){
	mText = text;
}

void EditBox::ClearText(){
	mString[0] = 0;
	mCaret = 0;
	mSelectStart = mSelectEnd = 0;
	UpdateText();
}

void EditBox::SetText(const char* text){
	strcpy_s(mString, 256, text);
	mCaret = strlen(mString);
	mSelectStart = mSelectEnd = 0;
	UpdateText();
}

const char* EditBox::GetText(){
	return mString;
}

int EditBox::GetTextHeight(){
	const char* pch = mString;
	int height = 0;
	while(*pch){
		int h = mText->GetCharHeight(*pch);
		if(h > height) height = h;
		++pch;
	}

	return height;
}

int EditBox::GetCaretFromX(int x){
	int caret = 0;
	if(x < 0) return 0;
	if(!mPassword){
		const char* pch = mString;
		int pos = 0, w;
		while(*pch){
			w = mText->GetCharWidth(*pch);
			if(pos + w > x) break;
			pos += w;
			++caret;
			++pch;
		}
	}else{
		caret = x / mText->GetCharWidth('*');
		int len = strlen(mString);
		if(caret > len) caret = len;
	}

	return caret;
}

int EditBox::GetXFromCaret(int caret){
	int x = 0;
	if(!mPassword){
		for(int i = 0; i < caret; ++i)
			x += mText->GetCharWidth(mString[i]);
	}else{
		x = mText->GetCharWidth('*') * caret;
	}

	return x;
}

void EditBox::DeleteSelection(){
	int len = strlen(mString);
	int left, right, dc;
	left = mSelectStart;
	right = mSelectEnd;
	dc = right - left;
	memcpy_s(mString, 256, mString, left);
	memcpy_s(mString + left, 256, mString + right, len - right);
	mString[len - dc] = 0;
	UpdateText();
	mCaret = left;
	mSelectStart = mSelectEnd = 0;
}

void EditBox::OnKeyDown(int key){
	switch(key){
		case VK_DELETE:
			if(mSelectStart != mSelectEnd){
				DeleteSelection();
			}else{
				int len = strlen(mString) - 1;
				if(len < 0 || mCaret > len) return;
				memcpy_s(mString, 256, mString, mCaret);
				memcpy_s(mString + mCaret, 256, mString + mCaret + 1, len - mCaret);
				mString[len] = 0;
				UpdateText();
			}
		break;
		case VK_HOME:
			mCaret = 0;
		break;
		case VK_END:
			mCaret = strlen(mString);
		break;
		case VK_LEFT:
			if(mSelectStart != mSelectEnd){
				mCaret = mSelectStart;
				mSelectStart = mSelectEnd = 0;
			}else if(mCaret > 0)
				--mCaret;
		break;
		case VK_RIGHT:
		{
			int len = strlen(mString);
			if(mSelectStart != mSelectEnd){
				mCaret = mSelectEnd;
				mSelectStart = mSelectEnd = 0;
			}else if(mCaret < len)
				++mCaret;
		}
		break;
	}
}

void EditBox::OnChar(int key){
	if(key >= 32 && key <= 126){
		if(mSelectStart != mSelectEnd){
			int left, right, dc;
			left = mSelectStart;
			right = mSelectEnd;
			dc = right - left;
			int len = strlen(mString) + 1 - dc;
			memcpy_s(mString, 256, mString, left);
			mString[left] = char(key);
			memcpy_s(mString + right - dc + 1, 256, mString + right, len - right + dc);
			mString[len] = 0;
			UpdateText();
			mCaret = left + 1;
			mSelectStart = mSelectEnd = 0;
		}else{
			int len = strlen(mString) + 1;
			memcpy_s(mString, 256, mString, mCaret);
			memcpy_s(mString + mCaret + 1, 256, mString + mCaret, len - mCaret);
			mString[mCaret] = char(key);
			mString[len] = 0;
			UpdateText();
			mCaret++;
		}
	}else if(key == VK_BACK){
		if(mSelectStart != mSelectEnd){
			DeleteSelection();
		}else if(mCaret > 0){
			int len = strlen(mString);
			memcpy_s(mString, 256, mString, mCaret - 1);
			memcpy_s(mString + (mCaret - 1), 256, mString + mCaret, len - mCaret);
			mString[len - 1] = 0;
			--mCaret;
			UpdateText();
		}
	}
}

void EditBox::UpdateText(){
	if(!mPassword){
		mText->SetText(mString);
	}else{
		char buffer[256];
		int len = strlen(mString);
		for(int i = 0; i < len; ++i)
			buffer[i] = '*';
		buffer[len] = 0;
		mText->SetText(buffer);
	}

	if(mString[0] == 0)
		mSelectEnd = mSelectStart = mCaret = 0;
}

void EditBox::AlignSelection(){
	if(mSelectEnd < mSelectStart){
		int tmp = mSelectEnd;
		mSelectEnd = mSelectStart;
		mSelectStart = tmp;
	}
}