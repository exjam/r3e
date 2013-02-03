#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "IControl.hpp"
#include "IRender.hpp"
#include "Array.hpp"
#include "Caption.hpp"

class Dialog : public IControl {
public:
	Dialog();
	virtual ~Dialog();

	virtual void OnCreated();
	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);
	virtual IControl* GetControlByID(int id);

	bool CreateFromXML(const char* xml);
	void AddChild(IControl* child);

	INT_PROPERTY(Modal);
	INT_PROPERTY(ShowSoundID);
	INT_PROPERTY(HideSoundID);
	INT_PROPERTY(Extent);
	PNT_PROPERTY(DefaultPosition);
	PNT_PROPERTY(DefaultAdjustPosition);
	INT_PROPERTY(DefaultVisible);

private:
	Array<IControl*, 5> mChildren;
	Caption* mCaption;
	Point mCaptionClicked;
};

#endif