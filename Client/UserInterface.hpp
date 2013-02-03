#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "Array.hpp"
#include "MsgBox.hpp"
#include "Window.hpp"

#include "..\RGE\Dialog.hpp"
#include "..\RGE\UiState.hpp"
#include "..\R3E\UiRender.hpp"
#include "..\RGE\IRender.hpp"
#include "..\RGE\ImageRes.hpp"
#include "..\RGE\TranslateWin32Event.hpp"

extern class UserInterface* gInterface;

class UserInterface {
public:
	UserInterface();
	~UserInterface();

	void Init();
	
	void CreateDialogs();

	Dialog* FindDialog(int id);
	void AddDialog(Dialog* dlg);
	void RemoveDialog(Dialog* dlg);

	int Update();
	void Render();

	int HandleEvent(GuiEvent* gevt);
	int HandleEvent(UINT uiMsg, WPARAM wParam, LPARAM lParam);

	void ShowMsgBox(const char* msg, int buttons);
	void HideMsgBox();

private:
	Array<Dialog*, 5> mDialogs;
	Point mMousePos;
	MsgBox* mMsgBox;
};

#endif
