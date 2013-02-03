#ifndef FONTS_HPP
#define FONTS_HPP

#include "..\R3E\FontList.hpp"

enum {
	FONT_NORMAL	= 0,
	FONT_LARGE,
	FONT_SMALL,
	FONT_NORMAL_BOLD,
	FONT_LARGE_BOLD,
	FONT_SMALL_BOLD,
	FONT_TITLE,
	FONT_DESCRIPTION,
	FONT_NORMAL_OUTLINE,
	FONT_OUTLINE_18_BOLD,
	FONT_OUTLINE_14_BOLD,
	FONT_OUTLINE_24_BOLD,
	FONT_OUTLINE_16_BOLD,
	FONT_OUTLINE_11_BOLD,
	MAX_FONT,
};

class GameFonts {
public:
	static void Load(){
		static const char* paths[] = {
			"R3EFonts\\font_normal.fnt",
			"R3EFonts\\font_large.fnt",
			"R3EFonts\\font_small.fnt",
			"R3EFonts\\font_normal_bold.fnt",
			"R3EFonts\\font_large_bold.fnt",
			"R3EFonts\\font_small_bold.fnt",
			0,
			0,
			"R3EFonts\\font_normal_outline.fnt",
			"R3EFonts\\font_outline18_bold.fnt",
			"R3EFonts\\font_outline14_bold.fnt",
			"R3EFonts\\font_outline24_bold.fnt",
			"R3EFonts\\font_outline16_bold.fnt",
			"R3EFonts\\font_outline11_bold.fnt",
			0
		};

		for(unsigned int i = 0; i < MAX_FONT; ++i){
			if(!paths[i]) continue;
			FontList::Instance().AddFont(i, paths[i]);
		}
	}

private:
};

#endif
