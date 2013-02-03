#ifndef LISTBOX_HPP
#define LISTBOX_HPP

#include "IRender.hpp"
#include "IControl.hpp"
#include "String.hpp"
#include "Array.hpp"
#include "IText.hpp"
#include "Colour.hpp"
#include "IScrollModel.hpp"

class ListBox : public IControl, public IScrollModel {
private:
	struct ListItem {
		String mValue;
		IText* mText;
		Colour mColour;
	};

public:
	ListBox();
	virtual ~ListBox();

	virtual void Render();
	virtual int HandleEvent(MouseEvent* evt);
	virtual int HandleEvent(KeyboardEvent* evt);
	virtual int Update(Point mouse);

	int AddItem(const char* text, Colour colour = Colour::White);
	void DeleteItem(int idx);

	const char* GetItem(int idx);
	void SetItem(int idx, const char* text);

	void Clear();

	unsigned int ItemCount();

	INT_PROPERTY(Extent);
	INT_PROPERTY(LineSpace);
	INT_PROPERTY(Selectable);
	INT_PROPERTY(CharWidth);
	INT_PROPERTY(CharHeight);
	INT_PROPERTY(MaxSize);
	INT_PROPERTY(OwnerDraw);
	INT_PROPERTY(Font);
	UINT_PROPERTY(Selected);

	virtual void SetValue(int val){
		mLineIdx = val;
	}

	virtual int GetValue(){
		return mLineIdx;
	}

	virtual int GetMax(){
		int max = mItems.Size() - mFitLines;
		return (max > 0)?max:0;
	}

	virtual int GetMin(){
		return 0;
	}

private:
	int mLineIdx;
	int mFitLines;
	bool mAutoResize;
	Array<ListItem, 5> mItems;
};

#endif
