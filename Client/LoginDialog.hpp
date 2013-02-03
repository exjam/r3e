#ifndef LOGIN_DIALOG_HPP
#define LOGIN_DIALOG_HPP

#include "..\RGE\Dialog.hpp"

class Button;
class EditBox;

class LoginDialog : public Dialog {
public:
	LoginDialog();
	~LoginDialog();

	static LoginDialog* Create();

	virtual void OnCreated();

	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

private:
	void EmitLoginEvent();

private:
	EditBox* mUsername;
	EditBox* mPassword;
	Button* mButtonOK;
	Button* mButtonExit;
};

#endif