#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "IRender.hpp"
#include "IControl.hpp"
#include "TSI.hpp"

class Image : public IControl {
public:
	Image();
	virtual ~Image();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	INT_PROPERTY(AlphaValue);
	INT_PROPERTY(ScaleWidth);
	INT_PROPERTY(ScaleHeight);
	INT_PROPERTY(SizeFit);
	STR_PROPERTY(GraphicID);
	CUSTOM_PROPERTY(ROSE::TSI::Graphic*, Graphic);
};

#endif