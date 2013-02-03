#ifndef GUI_EVENT_HPP
#define GUI_EVENT_HPP

#define GUIEVENT_DATA_SIZE 64

enum EventType {
	EVT_MOUSE,
	EVT_KEYBOARD,
	EVT_WINDOW,
};

struct GuiEvent {
	int _evt_type;
};

#endif
