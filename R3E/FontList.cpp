#include "Global.h"

#include "FontList.hpp"
#include "ResourceManager.hpp"

FontList::FontList(){}
FontList::~FontList(){}

void FontList::AddFont(int index, const char* path){
	FontDef* font = &mFonts.Add();
	font->mIdx = index;
	font->mPath = path;
	font->mFont = FONT_MGR().Load(path);
}

SmartPointer<Font> FontList::GetFont(int index) const {
	for(unsigned int i = 0; i < mFonts.Size(); ++i)
		if(mFonts[i].mIdx == index)
			return mFonts[i].mFont;

	return NULL;
}
