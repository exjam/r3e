#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "GuiEvent.hpp"

enum KeboardEventType {
	KEY_DOWN,
	KEY_UP,
	KEY_CHAR,
};

struct KeyboardEvent : public GuiEvent {
	KeyboardEvent(){ _evt_type = EVT_KEYBOARD; }

	KeboardEventType type;
	int key;
};

#endif
