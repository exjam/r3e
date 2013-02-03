#include "Global.h"

#include "Text.hpp"

Text::Text() : mVertices(0) {}
Text::~Text(){
	SAFE_DELETE(mVertices);
}

void Text::SetFont(const SmartPointer<Font>& font){
	mFont = font;
}

int Text::GetWidth(){
	int width = 0;
	const char* pch = mText;
	if(!pch) return width;
	while(*pch){
		width += mFont->GetCharWidth(*pch);
		++pch;
	}

	return width;
}

int Text::GetHeight(){
	int height = 0;
	const char* pch = mText;
	if(!pch) return height;
	while(*pch){
		int tmp = mFont->GetCharHeight(*pch);
		if(tmp > height) height = tmp;
		++pch;
	}

	return height;
}

int Text::GetCharHeight(char chr){
	return mFont->GetCharHeight(chr);
}

int Text::GetCharWidth(char chr){
	return mFont->GetCharWidth(chr);
}

void Text::SetText(const char* text){
	mText = text;
	mFont->GenerateVertices(mVertices, text);
}

const char* Text::GetText(){
	return mText;
}

void Text::SetPosition(int x, int y){
	mPosition.x = (float)x;
	mPosition.y = (float)y;
}

void Text::Render(float depth){
	int len = mText.Length();
	if(len == 0) return;
	len *= 4;

	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, -depth);
	mFont->Bind();
	glInterleavedArrays(GL_T2F_V3F, 0, mVertices);
	glDrawArrays(GL_QUADS, 0, len);
	glPopMatrix();
}