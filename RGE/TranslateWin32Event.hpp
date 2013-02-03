#ifndef TRANS_WIN32_EVENT_HPP
#define TRANS_WIN32_EVENT_HPP

#include "GuiEvent.hpp"
#include "MouseEvent.hpp"
#include "KeyboardEvent.hpp"
#include "WindowEvent.hpp"

static GuiEvent* TranslateWin32Event(UINT uiMsg, WPARAM wParam, LPARAM lParam){
	static MouseEvent mouseEvt;
	static KeyboardEvent keyEvt;
	static WindowEvent winEvt;

	if(uiMsg == WM_MOUSEMOVE){
		mouseEvt.type = MOUSE_MOVE;
		mouseEvt.button = NO_BUTTON;

		if(wParam & MK_LBUTTON)
			mouseEvt.button = (MouseEventButton)(mouseEvt.button | LBUTTON);

		if(wParam & MK_RBUTTON)
			mouseEvt.button = (MouseEventButton)(mouseEvt.button | RBUTTON);

		if(wParam & MK_MBUTTON)
			mouseEvt.button = (MouseEventButton)(mouseEvt.button | MBUTTON);

		mouseEvt.abspos = Point(lParam);
		mouseEvt.pos = mouseEvt.abspos;
		return &mouseEvt;
	}else if(uiMsg == WM_MOUSEWHEEL){
		mouseEvt.type = WHEEL;
		mouseEvt.button = (MouseEventButton)GET_WHEEL_DELTA_WPARAM(wParam);
		mouseEvt.abspos = Point(lParam);
		mouseEvt.pos = mouseEvt.abspos;
		return &mouseEvt;
	}else if(uiMsg >= WM_LBUTTONDOWN && uiMsg <= WM_MBUTTONDBLCLK){
		if(uiMsg == WM_LBUTTONDBLCLK)
			uiMsg -= WM_LBUTTONDOWN + 1 - 1;
		else
			uiMsg -= WM_LBUTTONDOWN;
		mouseEvt.type = (MouseEventType)(1 + (uiMsg % 3));
		mouseEvt.button = (MouseEventButton)(1 << (uiMsg / 3));
		mouseEvt.abspos = Point(lParam);
		mouseEvt.pos = mouseEvt.abspos;
		return &mouseEvt;
	}else if(uiMsg == WM_KEYDOWN){
		keyEvt.type = KEY_DOWN;
		keyEvt.key = wParam;
		return &keyEvt;
	}else if(uiMsg == WM_KEYUP){
		keyEvt.type = KEY_UP;
		keyEvt.key = wParam;
		return &keyEvt;
	}else if(uiMsg == WM_CHAR){
		keyEvt.type = KEY_CHAR;
		keyEvt.key = wParam;
		return &keyEvt;
	}else if(uiMsg == WM_SIZE){
		winEvt.type = WIN_RESIZE;
		winEvt.width = LOWORD(lParam);
		winEvt.height = HIWORD(lParam);
		return &winEvt;
	}

	return NULL;
}

#endif
