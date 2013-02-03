#ifndef TABBEDPANE_HPP
#define TABBEDPANE_HPP

#include "IControl.hpp"
#include "IRender.hpp"
#include "Array.hpp"
#include "Tab.hpp"
#include "RadioBox.hpp"

class TabbedPane : public IControl {
public:
	TabbedPane();
	virtual ~TabbedPane();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	virtual IControl* GetControlByID(int id);

	void AddTab(Tab* child);
	unsigned int ActiveTab();
	void SetActiveTab(unsigned int tab);

private:
	Array<Tab*, 2> mTabs;
	unsigned int mActiveTab;
	RadioBox mRadioBox;
};

#endif