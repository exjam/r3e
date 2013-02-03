#ifndef FONTLIST_HPP
#define FONTLIST_HPP

#include "Font.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "SmartPointer.hpp"

class FontList {
private:
	struct FontDef {
		int mIdx;
		String mPath;
		SmartPointer<Font> mFont;
	};

public:
	FontList();
	~FontList();

	static FontList& Instance(){
		static FontList self;
		return self;
	}
	
	void AddFont(int index, const char* path);
	SmartPointer<Font> GetFont(int index) const;

private:
	Array<FontDef, 20> mFonts;
};

#endif