#ifndef RADIO_BUTTON_HPP
#define RADIO_BUTTON_HPP

#include "IRender.hpp"
#include "IControl.hpp"
#include "TSI.hpp"

class RadioBox;

class RadioButton : public IControl {
private:
	enum ButtonState {
		BS_NORMAL,
		BS_MOUSEOVER,
		BS_CLICKED,
	};

public:
	RadioButton();
	virtual ~RadioButton();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	STR_PROPERTY(NormalGID);
	STR_PROPERTY(OverGID);
	STR_PROPERTY(DownGID);
	INT_PROPERTY(RadioBoxID);
	INT_PROPERTY(DisableSoundID);
	INT_PROPERTY(Ghost);
	INT_PROPERTY(ButtonState);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, NormalGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, OverGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, DownGraphic);
	CUSTOM_PROPERTY(RadioBox*, ParentBox);
};

#endif