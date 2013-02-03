#ifndef SCROLL_BAR_HPP
#define SCROLL_BAR_HPP

#include "IControl.hpp"

enum {
	SBT_VERTICAL,
	SBT_HORIZONTAL,
};

class ScrollBox;
class IScrollModel;

class ScrollBar : public IControl {
public:
	ScrollBar();
	virtual ~ScrollBar();

	virtual void Render();
	virtual int Update(Point mouse);
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

	void ScrollToTop();
	void ScrollToBottom();

	void SetScrollBox(ScrollBox* box);
	void SetModel(IScrollModel* model);
	void SetScrollPercent(float percent);

	INT_PROPERTY(ListBoxID);
	INT_PROPERTY(Type);

protected:
	ScrollBox* mScrollBox;
	IScrollModel* mModel;
};

#endif