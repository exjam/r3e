#ifndef LABEL_HPP
#define LABEL_HPP

#include "IText.hpp"
#include "IRender.hpp"
#include "IControl.hpp"

enum Align {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class Label : public IControl {
public:
	Label(){
		mText = 0;
		mFont = 0;
		mAlignX = 0;
		mAlign = ALIGN_LEFT;
		mString[0] = 0;
		mAutosize = false;
		mDepth = 1.0f;
	}

	virtual ~Label(){
		if(mText)
			gUiRender->FreeText(mText);
	}

	virtual void Render(){
		if(!Visible()) return;
		if(!mText) SetFont(0);

		int x = mPosition.x + mAlignX;
		int y = mPosition.y;

		gUiRender->PushTranslate(x, y);
		gUiRender->SetDrawColour(&mColour);
		gUiRender->DrawText(mText, mDepth);
		gUiRender->ResetDrawColour();
		gUiRender->PopTranslate(x, y);
	}

	virtual int Update(Point mouse){
		return Visible() && IsInside(mouse);
	}

	virtual int HandleEvent(MouseEvent* /*evt*/){
		return 0;
	}

	virtual int HandleEvent(KeyboardEvent* /*evt*/){
		return 0;
	}

	void SetAlign(int align){
		mAlign = align;
		if(align == ALIGN_LEFT)
			mAlignX = 0;
		else if(align == ALIGN_CENTER)
			mAlignX = (SizeX() / 2) - (mText->GetWidth() / 2);
		else if(align == ALIGN_RIGHT)
			mAlignX = SizeX() - mText->GetWidth();
	}

	void SetFont(int font){
		if(mText)
			gUiRender->FreeText(mText);

		mText = gUiRender->CreateTextWithFont(font);
		mText->SetText(mString);
		SetSizeY(mText->GetHeight());

		if(mAutosize)
			SetSizeX(mText->GetWidth());
	}

	void SetText(const char* text){
		if(text) strcpy_s(mString, 256, text);
		else mString[0] = 0;

		mText->SetText(mString);
		SetSizeY(mText->GetHeight());

		if(mAutosize)
			SetSizeX(mText->GetWidth());
	}

	void SetAutosize(bool autosize){
		mAutosize = autosize;
	}

	int Font(){
		return mFont;
	}

	const char* Text(){
		return mString;
	}

	const Colour& GetColour(){
		return mColour;
	}

	void SetColour(const Colour& tmp){
		mColour = tmp;
	}

	void SetDepth(float depth){
		mDepth = depth;
	}

private:
	float mDepth;
	bool mAutosize;
	int mAlign;
	int mAlignX;
	Colour mColour;
	int mFont;
	IText* mText;
	char mString[256];
};

#endif
