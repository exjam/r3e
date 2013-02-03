#ifndef RADIOBOX_HPP
#define RADIOBOX_HPP

#include "IRender.hpp"
#include "IControl.hpp"
#include "RadioButton.hpp"

class RadioBox : public IControl {
public:
	RadioBox();
	virtual ~RadioBox();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	void AddButton(RadioButton* btn);

	void RemoveButton(RadioButton* btn);
	void SetSelected(RadioButton* opt);

	UINT_PROPERTY(SelectedIdx);

private:
	Array<RadioButton*, 2> mChildren;
};

#endif