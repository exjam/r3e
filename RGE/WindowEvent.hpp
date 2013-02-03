#ifndef WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "GuiEvent.hpp"

enum WindowEventType {
	WIN_RESIZE,
};

struct WindowEvent : public GuiEvent {
	WindowEvent(){ _evt_type = EVT_WINDOW; }

	WindowEventType type;
	int width;
	int height;
};

#endif
