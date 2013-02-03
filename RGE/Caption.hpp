#ifndef CAPTION_HPP
#define CAPTION_HPP

#include "IControl.hpp"

class Caption : public IControl {
public:
	Caption();
	virtual ~Caption();

	virtual void Render();

	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);

	virtual int Update(Point mouse);

	bool IsClicked();

private:
	bool mClicked;
};

#endif