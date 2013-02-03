#ifndef CHARACTER_SELECT_DIALOG_HPP
#define CHARACTER_SELECT_DIALOG_HPP

#include "..\RGE\Dialog.hpp"

class Button;

class CharacterSelectDialog : public Dialog {
public:
	CharacterSelectDialog();
	~CharacterSelectDialog();

	static CharacterSelectDialog* Create();

	virtual void OnCreated();
	virtual int HandleEvent(MouseEvent* evt);

private:
	Button* mButtonCreate;
	Button* mButtonDelete;
	Button* mButtonStart;
	Button* mButtonExit;
};

#endif