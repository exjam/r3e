#ifndef SCROLL_BOX_HPP
#define SCROLL_BOX_HPP

#include "TSI.hpp"
#include "IControl.hpp"
#include "ScrollBar.hpp"

class ScrollBar;

class ScrollBox : public IControl {
public:
	ScrollBox() : mTickMove(0), mBlink(0), mBlinkSwapTime(0), mBlinkGraphic(0), mGraphic(0), mClicked(false) {
	}

	virtual ~ScrollBox(){
	}

	virtual void Render(){
		if(!Visible() || !mGraphic) return;
		gUiRender->PushTranslate(mPosition.x, mPosition.y);
		mGraphic->Render();
		gUiRender->PopTranslate(mPosition.x, mPosition.y);
	}

	virtual int Update(Point mouse){
		if(!Visible()) return 0;

		if(mClicked){
			Point move = mouse - mClickPos;
			mPosition.y += move.y;
			if(mPosition.y < 0) mPosition.y = 0;
			if(mPosition.y > mMaxY) mPosition.y = mMaxY;
			mClickPos = mouse;
			mScrollBar->SetScrollPercent(float(mPosition.y) / float(mMaxY));
			return ID();
		}

		return 0;
	}

	virtual int HandleEvent(MouseEvent* evt){
		if(!Visible()) return 0;
		if(evt->button != LBUTTON) return 0;

		if(evt->type == BUTTON_DOWN){
			if(!IsInside(evt->pos)) return 0;
			mClicked = true;
			mClickPos = evt->pos;
			return 1;
		}else if(evt->type == BUTTON_UP)
			mClicked = false;

		return IsInside(evt->pos);
	}

	virtual int HandleEvent(KeyboardEvent* /*evt*/){
		return 0;
	}

	void SetScrollBar(ScrollBar* bar){
		mScrollBar = bar;
		mMaxY = mScrollBar->SizeY() - SizeY();
	}

	void ScrollToTop(){
		mClicked = false;
		mPosition.y = 0;
		mScrollBar->SetScrollPercent(0.0f);
	}

	void ScrollToBottom(){
		mClicked = false;
		mPosition.y = mMaxY;
		mScrollBar->SetScrollPercent(1.0f);
	}

	INT_PROPERTY(TickMove);

	INT_PROPERTY(Blink);
	INT_PROPERTY(BlinkSwapTime);

	STR_PROPERTY(BlinkGraphicID);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, BlinkGraphic);

	STR_PROPERTY(GraphicID);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, Graphic);

private:
	ScrollBar* mScrollBar;
	bool mClicked;
	Point mClickPos;
	int mMaxY;
};

#endif
