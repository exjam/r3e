#ifndef MSGBOX_HPP
#define MSGBOX_HPP

#include "..\RGE\Dialog.hpp"

class Image;
class Button;
class ListBox;

class MsgBox : public Dialog {
public:
	enum Buttons {
		BUTTON_OK = 1,
		BUTTON_CANCEL = 2,
	};

public:
	MsgBox();
	~MsgBox();

	static MsgBox* Create();

	virtual void OnCreated();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);

	void SetButtons(int buttons);
	void SetMessage(const char* message);

private:
	Button* mButtonOK;
	Button* mButtonCancel;
	Image* mImageTop;
	Image* mImageMiddle;
	Image* mImageBottom;
	ListBox* mListBox;
};

#endif
