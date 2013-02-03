#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "IRender.hpp"
#include "IControl.hpp"
#include "TSI.hpp"

class Button : public IControl {
private:
	enum ButtonState {
		BS_NORMAL,
		BS_MOUSEOVER,
		BS_CLICKED,
	};

public:
	Button();
	virtual ~Button();

	virtual void Render();

	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

	virtual int Update(Point mouse);

	STR_PROPERTY(NormalGID);
	STR_PROPERTY(OverGID);
	STR_PROPERTY(DownGID);
	STR_PROPERTY(BlinkGID);
	STR_PROPERTY(DisableGID);
	INT_PROPERTY(OverSoundID);
	INT_PROPERTY(ClickSoundID);
	INT_PROPERTY(NoImage);
	INT_PROPERTY(ButtonState);
	BOOL_PROPERTY(Enabled);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, NormalGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, OverGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, DownGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, BlinkGraphic);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, DisableGraphic);
};

#endif