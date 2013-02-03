#ifndef TAB_HPP
#define TAB_HPP

#include "IControl.hpp"
#include "IRender.hpp"
#include "Array.hpp"
#include "RadioButton.hpp"

class TabbedPane;

class Tab : public IControl {
public:
	Tab(TabbedPane* parent);
	virtual ~Tab();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	virtual IControl* GetControlByID(int id);

	void AddChild(IControl* child);

	CUSTOM_PROPERTY(RadioButton*, TabButton);

private:
	Array<IControl*, 5> mChildren;
	TabbedPane* mParent;
};

#endif