#define TIXML_USE_TICPP
#include "ticpp.h"

#include "Global.h"

#include "Dialog.hpp"
#include "File.hpp"
#include "Log.hpp"
#include "ScopedPointer.hpp"
#include "ImageRes.hpp"

#include "Image.hpp"
#include "Button.hpp"
#include "EditBox.hpp"
#include "Caption.hpp"
#include "ListBox.hpp"
#include "RadioBox.hpp"
#include "ScrollBox.hpp"
#include "ScrollBar.hpp"
#include "TabbedPane.hpp"
#include "RadioButton.hpp"

IControl* CreateControlFromXML(ticpp::Element* elem, Dialog* dlg);

ScrollBox* CreateScrollBoxFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	ScrollBox* box = new ScrollBox();

	box->SetName(elem->GetAttribute("NAME").c_str());
	box->SetGraphicID(elem->GetAttribute("GID").c_str());
	box->SetBlinkGraphicID(elem->GetAttribute("BLINKGID").c_str());

	if(elem->GetAttribute("ID", &tmpInt))			box->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))			box->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))			box->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))		box->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))		box->SetSizeY(tmpInt);

	if(elem->GetAttribute("TICKMOVE", &tmpInt))		box->SetTickMove(tmpInt);
	if(elem->GetAttribute("BLINK", &tmpInt))		box->SetBlink(tmpInt);
	if(elem->GetAttribute("BLINKSWAPTIME", &tmpInt))box->SetBlinkSwapTime(tmpInt);

	box->SetGraphic(ImageRes::Instance().GetGraphic(box->GraphicID()));
	box->SetBlinkGraphic(ImageRes::Instance().GetGraphic(box->BlinkGraphicID()));

	return box;
}

ScrollBar* CreateScrollBarFromXML(ticpp::Element* elem, Dialog* dlg){
	int tmpInt;
	ScrollBar* bar = new ScrollBar();

	bar->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))		bar->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		bar->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		bar->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	bar->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	bar->SetSizeY(tmpInt);
	if(elem->GetAttribute("TYPE", &tmpInt))		bar->SetType(tmpInt);
	if(elem->GetAttribute("LISTBOXID", &tmpInt))bar->SetListBoxID(tmpInt);
	
	for(ticpp::Element* child = elem->FirstChildElement(false); child; child = child->NextSiblingElement(false)){
		if(child->Value() == "SCROLLBOX"){
			ScrollBox* box = (ScrollBox*)CreateControlFromXML(child, dlg);
			box->SetScrollBar(bar);
			bar->SetScrollBox(box);
		}
	}

	if(int id = bar->ListBoxID()){
		ListBox* lst = (ListBox*)dlg->GetControlByID(id);
		bar->SetModel(lst);
	}

	return bar;
}

Tab* CreateTabFromXML(ticpp::Element* elem, TabbedPane* parent, Dialog* dlg){
	int tmpInt;
	Tab* tab = new Tab(parent);

	tab->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))		tab->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		tab->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		tab->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	tab->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	tab->SetSizeY(tmpInt);

	for(ticpp::Element* child = elem->FirstChildElement(false); child; child = child->NextSiblingElement(false)){
		if(child->Value() == "TABBUTTON"){
			tab->SetTabButton((RadioButton*)CreateControlFromXML(child, dlg));
		}else if(child->Value() == "RADIOBUTTON"){
			RadioButton* btn = (RadioButton*)CreateControlFromXML(child, dlg);
			if(int id = btn->RadioBoxID()){
				RadioBox* box = (RadioBox*)tab->GetControlByID(id);
				if(box && box->ControlType() == CT_RADIOBOX)
					box->AddButton(btn);
			}

			tab->AddChild(btn);
		}else{
			tab->AddChild(CreateControlFromXML(child, dlg));
		}
	}

	return tab;
}

TabbedPane* CreateTabbedPaneFromXML(ticpp::Element* elem, Dialog* dlg){
	int tmpInt;
	TabbedPane* pane = new TabbedPane();

	pane->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))		pane->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		pane->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		pane->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	pane->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	pane->SetSizeY(tmpInt);
	
	for(ticpp::Element* child = elem->FirstChildElement("TAB", false); child; child = child->NextSiblingElement("TAB", false)){
		pane->AddTab(CreateTabFromXML(child, pane, dlg));
	}

	pane->SetActiveTab(0);

	return pane;
}

ListBox* CreateListBoxFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	ListBox* box = new ListBox();

	box->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))			box->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))			box->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))			box->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))		box->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))		box->SetSizeY(tmpInt);
	if(elem->GetAttribute("CHARWIDTH", &tmpInt))	box->SetCharWidth(tmpInt);
	if(elem->GetAttribute("CHARHEIGHT", &tmpInt))	box->SetCharHeight(tmpInt);
	if(elem->GetAttribute("FONT", &tmpInt))			box->SetFont(tmpInt);
	if(elem->GetAttribute("EXTENT", &tmpInt))		box->SetExtent(tmpInt);
	if(elem->GetAttribute("LINESPACE", &tmpInt))	box->SetLineSpace(tmpInt);
	if(elem->GetAttribute("SELECTABLE", &tmpInt))	box->SetSelectable(tmpInt);
	if(elem->GetAttribute("MAXSIZE", &tmpInt))		box->SetMaxSize(tmpInt);
	if(elem->GetAttribute("OWNERDRAW", &tmpInt))	box->SetOwnerDraw(tmpInt);

	return box;
}

EditBox* CreateEditBoxFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	EditBox* box = new EditBox();

	box->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))			box->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))			box->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))			box->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))		box->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))		box->SetSizeY(tmpInt);
	if(elem->GetAttribute("CHARWIDTH", &tmpInt))	box->SetCharWidth(tmpInt);
	if(elem->GetAttribute("CHARHEIGHT", &tmpInt))	box->SetCharHeight(tmpInt);
	if(elem->GetAttribute("NUMBER", &tmpInt))		box->SetNumber(tmpInt);
	if(elem->GetAttribute("LIMITTEXT", &tmpInt))	box->SetLimitText(tmpInt);
	if(elem->GetAttribute("PASSWORD", &tmpInt))		box->SetPassword(tmpInt);
	if(elem->GetAttribute("HIDECURSOR", &tmpInt))	box->SetHideCursor(tmpInt);
	if(elem->GetAttribute("TEXTALIGN", &tmpInt))	box->SetTextAlign(tmpInt);
	if(elem->GetAttribute("MULTILINE", &tmpInt))	box->SetMultiline(tmpInt);
	if(elem->GetAttribute("TEXTCOLOR", &tmpInt))	box->SetTextColour(tmpInt);
	if(elem->GetAttribute("FONT", &tmpInt))			box->SetFont(tmpInt);
	
	box->SetTextElement(gUiRender->CreateTextWithFont(box->Font()));

	return box;
}

Caption* CreateCaptionFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	Caption* cap = new Caption();

	cap->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt))		cap->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		cap->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		cap->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	cap->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	cap->SetSizeY(tmpInt);
	
	return cap;
}

RadioButton* CreateRadioButtonFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	RadioButton* btn = new RadioButton();

	btn->SetName(elem->GetAttribute("NAME").c_str());
	btn->SetNormalGID(elem->GetAttribute("NORMALGID").c_str());
	btn->SetOverGID(elem->GetAttribute("OVERGID").c_str());
	btn->SetDownGID(elem->GetAttribute("DOWNGID").c_str());

	if(elem->GetAttribute("ID", &tmpInt))		btn->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		btn->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		btn->SetPositionY(tmpInt);
	if(elem->GetAttribute("GHOST", &tmpInt))	btn->SetGhost(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	btn->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	btn->SetSizeY(tmpInt);
	if(elem->GetAttribute("OFFSETX", &tmpInt))	btn->AddOffsetX(tmpInt);
	if(elem->GetAttribute("OFFSETY", &tmpInt))	btn->AddOffsetY(tmpInt);
	if(elem->GetAttribute("DISABLESID", &tmpInt))btn->SetDisableSoundID(tmpInt);
	if(elem->GetAttribute("RADIOBOXID", &tmpInt))btn->SetRadioBoxID(tmpInt);

	btn->SetNormalGraphic(ImageRes::Instance().GetGraphic(btn->NormalGID()));
	btn->SetOverGraphic(ImageRes::Instance().GetGraphic(btn->OverGID()));
	btn->SetDownGraphic(ImageRes::Instance().GetGraphic(btn->DownGID()));

	return btn;
}

Button* CreateButtonFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	Button* btn = new Button();

	btn->SetName(elem->GetAttribute("NAME").c_str());
	btn->SetNormalGID(elem->GetAttribute("NORMALGID").c_str());
	btn->SetOverGID(elem->GetAttribute("OVERGID").c_str());
	btn->SetDownGID(elem->GetAttribute("DOWNGID").c_str());
	btn->SetBlinkGID(elem->GetAttribute("BLINKGID").c_str());
	btn->SetDisableGID(elem->GetAttribute("DISABLEGID").c_str());

	if(elem->GetAttribute("ID", &tmpInt))		btn->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))		btn->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))		btn->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))	btn->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))	btn->SetSizeY(tmpInt);
	if(elem->GetAttribute("OFFSETX", &tmpInt))	btn->AddOffsetX(tmpInt);
	if(elem->GetAttribute("OFFSETY", &tmpInt))	btn->AddOffsetY(tmpInt);
	if(elem->GetAttribute("OVERSID", &tmpInt))	btn->SetOverSoundID(tmpInt);
	if(elem->GetAttribute("CLICKSID", &tmpInt))	btn->SetClickSoundID(tmpInt);
	if(elem->GetAttribute("NOIMAGE", &tmpInt))	btn->SetNoImage(tmpInt);

	btn->SetNormalGraphic(ImageRes::Instance().GetGraphic(btn->NormalGID()));
	btn->SetOverGraphic(ImageRes::Instance().GetGraphic(btn->OverGID()));
	btn->SetDownGraphic(ImageRes::Instance().GetGraphic(btn->DownGID()));
	btn->SetBlinkGraphic(ImageRes::Instance().GetGraphic(btn->BlinkGID()));
	btn->SetDisableGraphic(ImageRes::Instance().GetGraphic(btn->DisableGID()));

	return btn;
}

RadioBox* CreateRadioBoxFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	RadioBox* box = new RadioBox();

	box->SetName(elem->GetAttribute("NAME").c_str());
	if(elem->GetAttribute("ID", &tmpInt)) box->SetID(tmpInt);

	return box;
}

Image* CreateImageFromXML(ticpp::Element* elem, Dialog* /*dlg*/){
	int tmpInt;
	Image* img = new Image();

	img->SetName(elem->GetAttribute("NAME").c_str());
	img->SetGraphicID(elem->GetAttribute("GID").c_str());
	if(elem->GetAttribute("ID", &tmpInt))			img->SetID(tmpInt);
	if(elem->GetAttribute("X", &tmpInt))			img->SetPositionX(tmpInt);
	if(elem->GetAttribute("Y", &tmpInt))			img->SetPositionY(tmpInt);
	if(elem->GetAttribute("WIDTH", &tmpInt))		img->SetSizeX(tmpInt);
	if(elem->GetAttribute("HEIGHT", &tmpInt))		img->SetSizeY(tmpInt);
	if(elem->GetAttribute("OFFSETX", &tmpInt))		img->AddOffsetX(tmpInt);
	if(elem->GetAttribute("OFFSETY", &tmpInt))		img->AddOffsetY(tmpInt);
	if(elem->GetAttribute("ALPHAVALUE", &tmpInt))	img->SetAlphaValue(tmpInt);
	if(elem->GetAttribute("SCALEWIDTH", &tmpInt))	img->SetScaleWidth(tmpInt);
	if(elem->GetAttribute("SCALEHEIGHT", &tmpInt))	img->SetScaleHeight(tmpInt);
	img->SetGraphic(ImageRes::Instance().GetGraphic(img->GraphicID()));

	return img;
}

IControl* CreateControlFromXML(ticpp::Element* elem, Dialog* dlg){
	if(elem->Value() == "IMAGE"){
		return CreateImageFromXML(elem, dlg);
	}else if(elem->Value() == "IMAGETOP"){
		return CreateImageFromXML(elem, dlg);
	}else if(elem->Value() == "IMAGEMIDDLE"){
		return CreateImageFromXML(elem, dlg);
	}else if(elem->Value() == "IMAGEBOTTOM"){
		return CreateImageFromXML(elem, dlg);
	}else if(elem->Value() == "BUTTON"){
		return CreateButtonFromXML(elem, dlg);
	}else if(elem->Value() == "BUTTONOK"){
		return CreateButtonFromXML(elem, dlg);
	}else if(elem->Value() == "BUTTONCANCEL"){
		return CreateButtonFromXML(elem, dlg);
	}else if(elem->Value() == "CAPTION"){
		return CreateCaptionFromXML(elem, dlg);
	}else if(elem->Value() == "EDITBOX"){
		return CreateEditBoxFromXML(elem, dlg);
	}else if(elem->Value() == "TABBEDPANE"){
		return CreateTabbedPaneFromXML(elem, dlg);
	}else if(elem->Value() == "RADIOBOX"){
		return CreateRadioBoxFromXML(elem, dlg);
	}else if(elem->Value() == "RADIOBUTTON"){
		return CreateRadioButtonFromXML(elem, dlg);
	}else if(elem->Value() == "TABBUTTON"){
		return CreateRadioButtonFromXML(elem, dlg);
	}else if(elem->Value() == "LISTBOX"){
		return CreateListBoxFromXML(elem, dlg);
	}else if(elem->Value() == "SCROLLBAR"){
		return CreateScrollBarFromXML(elem, dlg);
	}else if(elem->Value() == "SCROLLBOX"){
		return CreateScrollBoxFromXML(elem, dlg);
	}

	return NULL;
}

bool Dialog::CreateFromXML(const char* idd){
	ScopedPointer<File> fh = gUiRender->OpenFile(idd);
	if(!fh) return false;
	char* xml = (char*)fh->ReadWholeFile();
	int tmpInt;
	bool result = true;
	
	try {
		ticpp::Document doc;
		doc.Parse(xml);

		ticpp::Element* root = doc.FirstChildElement("Root_Element");
		if(root->GetAttribute("ID", &tmpInt))			SetID(tmpInt);
		if(root->GetAttribute("X", &tmpInt))			SetPositionX(tmpInt);
		if(root->GetAttribute("Y", &tmpInt))			SetPositionY(tmpInt);
		if(root->GetAttribute("WIDTH", &tmpInt))		SetSizeX(tmpInt);
		if(root->GetAttribute("HEIGHT", &tmpInt))		SetSizeY(tmpInt);
		if(root->GetAttribute("MODAL", &tmpInt))		SetModal(tmpInt);
		if(root->GetAttribute("SHOWSID", &tmpInt))		SetShowSoundID(tmpInt);
		if(root->GetAttribute("HIDESID", &tmpInt))		SetHideSoundID(tmpInt);
		if(root->GetAttribute("EXTENT", &tmpInt))		SetExtent(tmpInt);
		if(root->GetAttribute("DEFAULT_X", &tmpInt))	SetDefaultPositionX(tmpInt);
		if(root->GetAttribute("DEFAULT_Y", &tmpInt))	SetDefaultPositionY(tmpInt);
		if(root->GetAttribute("ADJUST_X", &tmpInt))		SetDefaultAdjustPositionX(tmpInt);
		if(root->GetAttribute("ADJUST_Y", &tmpInt))		SetDefaultAdjustPositionY(tmpInt);
		if(root->GetAttribute("DEFAULT_VISIBLE", &tmpInt)) SetDefaultVisible(tmpInt);
		SetName(root->GetAttribute("NAME").c_str());
		
		for(ticpp::Element* child = root->FirstChildElement(false); child; child = child->NextSiblingElement(false)){
			if(child->Value() == "CAPTION"){
				mCaption = (Caption*)CreateControlFromXML(child, this);
			}else if(child->Value() == "RADIOBUTTON"){
				RadioButton* btn = (RadioButton*)CreateControlFromXML(child, this);
				if(int id = btn->RadioBoxID()){
					RadioBox* box = (RadioBox*)GetControlByID(id);
					if(box && box->ControlType() == CT_RADIOBOX)
						box->AddButton(btn);
				}

				AddChild(btn);
			}else{
				AddChild(CreateControlFromXML(child, this));
			}
		}

	}catch(ticpp::Exception& ex){
		LOG("TinyXML Exception %s", ex.what());
		result = false;
	}

	OnCreated();

	delete [] xml;
	return result;
}
