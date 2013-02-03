#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "Array.hpp"
#include "..\RGE\Label.hpp"
#include "..\RGE\IRender.hpp"
#include "..\RGE\ImageRes.hpp"
#include "..\RGE\IControl.hpp"

class ToolTip : public IControl {
public:
	ToolTip();
	virtual ~ToolTip();

	virtual void Render();
	virtual int Update(Point mouse);

	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

	void SetLineHeight(int height);
	void AddLine(const char* str, int font = 0, Colour colour = Colour(), int align = ALIGN_LEFT);

private:
	Array<Label*> mItems;
	ROSE::TSI::Graphic* mBackground;
	int mLineHeight;
};

#endif
