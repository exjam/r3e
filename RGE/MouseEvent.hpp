#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include "GuiEvent.hpp"
#include "Point.hpp"

enum MouseEventType {
	MOUSE_MOVE,
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_DBLCLICK,
	WHEEL,
};

enum MouseEventButton {
	NO_BUTTON = 0,
	LBUTTON = 1,
	RBUTTON = 1 << 1,
	MBUTTON = 1 << 2,
};

struct MouseEvent : public GuiEvent {
	MouseEvent(){ _evt_type = EVT_MOUSE; }

	Point abspos;
	Point pos;
	MouseEventType type;
	MouseEventButton button;
};

#endif
