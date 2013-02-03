#include "Global.h"

#include "Image.hpp"

Image::Image() : mGraphic(0), mAlphaValue(0),
	mScaleWidth(0), mScaleHeight(0), mSizeFit(0) {
	mControlType = CT_IMAGE;
}

Image::~Image(){}

void Image::Render(){
	if(!Visible() || !mGraphic) return;

	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	if(mAlphaValue) gUiRender->SetDrawColour(255, 255, 255, unsigned char(mAlphaValue));

	if(mScaleWidth || mScaleHeight) mGraphic->Render(mScaleWidth, mScaleHeight);
	else mGraphic->Render();


	if(mAlphaValue) gUiRender->ResetDrawColour();

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int Image::HandleEvent(MouseEvent* /*evt*/){
	return 0;
}

int Image::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int Image::Update(Point mouse){
	return Visible() && IsInside(mouse);
}
