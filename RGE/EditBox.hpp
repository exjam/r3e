#ifndef EDIT_BOX_HPP
#define EDIT_BOX_HPP

#include "IRender.hpp"
#include "IText.hpp"
#include "IControl.hpp"
#include "TSI.hpp"
#include "UiState.hpp"
#include <time.h>

class EditBox : public IControl {
public:
	EditBox();
	virtual ~EditBox();

	virtual void Render();
	virtual int Update(Point mouse);
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

	void ClearText();

	void SetFocus(bool focus);
	void SetText(const char* text);
	void SetTextElement(IText* text);

	const char* GetText();

protected:
	void UpdateText();

	void OnChar(int key);
	void OnKeyDown(int key);

	void AlignSelection();
	void DeleteSelection();

	int GetTextHeight();
	int GetCaretFromX(int x);
	int GetXFromCaret(int caret);

	INT_PROPERTY(CharWidth);
	INT_PROPERTY(CharHeight);
	INT_PROPERTY(Number);
	INT_PROPERTY(LimitText);
	INT_PROPERTY(Password);
	INT_PROPERTY(HideCursor);
	INT_PROPERTY(TextAlign);
	INT_PROPERTY(Multiline);
	INT_PROPERTY(TextColour);
	INT_PROPERTY(Font);

private:
	int mSelectStart;
	int mSelectEnd;
	int mCaret;

	clock_t mCaretTimer;
	bool mCaretVisible;

	bool mClicked;
	Point mClickPos;

	IText* mText;
	char mString[256];
};

#endif
