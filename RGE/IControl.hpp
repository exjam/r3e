#ifndef ICONTROL_HPP
#define ICONTROL_HPP

#include "MouseEvent.hpp"
#include "KeyboardEvent.hpp"
#include "Point.hpp"
#include "Rect.hpp"
#include "String.hpp"
#include "Colour.hpp"

#define CUSTOM_PROPERTY(type, name) \
	protected: type m##name; \
	public: type name(){ return m##name; }; \
	void Set##name(type tmp){ m##name = tmp; }

#define INT_PROPERTY(name) CUSTOM_PROPERTY(int, name)
#define UINT_PROPERTY(name) CUSTOM_PROPERTY(unsigned int, name)
#define BOOL_PROPERTY(name) CUSTOM_PROPERTY(bool, name)
#define FLOAT_PROPERTY(name) CUSTOM_PROPERTY(float, name)

#define STR_PROPERTY(name) \
	protected: String m##name; \
	public: const char* name(){ return m##name; }; \
	void Set##name(const char* tmp){ m##name = tmp; }

#define PNT_PROPERTY(name) \
	protected: Point m##name;  \
	public: Point name(){ return m##name; }; \
	int name##X(){ return m##name##.x; }; int name##Y(){ return m##name##.y; }; \
	void Set##name(const Point& tmp){ m##name = tmp; } \
	void Set##name##X(int x){ m##name##.x = x; } void Set##name##Y(int y){ m##name##.y = y; }

enum ControlType {
	CT_INVALID,
	CT_CAPTION,
	CT_DIALOG,
	CT_EDITBOX,
	CT_IMAGE,
	CT_RADIOBOX,
	CT_RADIOBUTTON,
	CT_TAB,
	CT_TABBEDPANE,
	CT_BUTTON,
	CT_LISTBOX,
};

class IControl {
public:
	IControl() : mID(0), mVisible(true), mControlType(CT_INVALID) {}
	virtual ~IControl(){}

	virtual void Render() = 0;
	virtual int HandleEvent(MouseEvent* evt) = 0;
	virtual int HandleEvent(KeyboardEvent* evt) = 0;
	virtual int Update(Point mouse) = 0;

	bool IsInside(Point& pos){
		Rect rect(mPosition.x, mPosition.y, mPosition.x + mSize.x, mPosition.y + mSize.y);
		return rect.Contains(pos);
	}

	void AddOffsetX(int x){
		mPosition.x += x;
	}

	void AddOffsetY(int y){
		mPosition.y += y;
	}

	PNT_PROPERTY(Position);
	PNT_PROPERTY(Size);
	INT_PROPERTY(ID);
	STR_PROPERTY(Name);
	CUSTOM_PROPERTY(bool, Visible);

	int ControlType(){
		return mControlType;
	}

	virtual IControl* GetControlByID(int id){
		return (id == mID)?this:NULL;
	}

protected:
	int mControlType;
};

#endif
