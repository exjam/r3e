#include "Global.h"

#include "ToolTip.hpp"
#include "DialogType.hpp"

ToolTip::ToolTip(){
	mLineHeight = 0;
	mBackground = ImageRes::Instance().GetGraphic("ID_BLACK_PANEL");
	SetID(DLG_TOOL_TIP);
}

ToolTip::~ToolTip(){
	mItems.DeleteValues();
}

void ToolTip::Render(){
	gUiRender->PushTranslate(mPosition.x, mPosition.y);

	mBackground->Render(SizeX(), SizeY());
	for(unsigned int i = 0; i < mItems.Size(); ++i)
		mItems[i]->Render();
	
	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

int ToolTip::HandleEvent(MouseEvent* /*evt*/){
	return 0;
}

int ToolTip::HandleEvent(KeyboardEvent* /*evt*/){
	return 0;
}

int ToolTip::Update(Point mouse){
	return Visible() && IsInside(mouse);
}

void ToolTip::SetLineHeight(int height){
	mLineHeight = height;
}

void ToolTip::AddLine(const char* str, int font, Colour colour, int align){
	Label* lbl = new Label();
	lbl->SetFont(font);
	lbl->SetAlign(align);
	lbl->SetAutosize(true);
	lbl->SetText(str);
	lbl->SetColour(colour);
	lbl->SetPositionY(SizeY());
	mItems.PushBack(lbl);

	int width = lbl->SizeX();
	if(width > SizeX()) SetSizeX(width);
	if(mLineHeight == 0)
		SetSizeY(lbl->SizeY() + SizeY());
	else
		SetSizeY(mItems.Size() * mLineHeight);
}
