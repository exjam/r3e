#ifndef GUIRENDER_HPP
#define GUIRENDER_HPP

#include "OpenGL.hpp"

#include "Texture.hpp"
#include "SmartPointer.hpp"

#include "Colour.hpp"
#include "IRender.hpp"

class IText;
class File;
struct Colour;

class IImage {
public:
	IImage(SmartPointer<Texture> tex) : mTex(tex){}
	~IImage(){}

	SmartPointer<Texture> mTex;
};

class UiRender : public IRender {
public:
	UiRender();
	virtual ~UiRender();

	virtual void ResetDrawColour();
	virtual void SetDrawColour(Colour* colour);
	virtual void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
	virtual void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	virtual void PushTranslate(int x, int y);
	virtual void PopTranslate(int x, int y);

	virtual void DrawText(IText* text, float depth = 1.0f);

	virtual void DrawLine(int _x1, int _y1, int _x2, int _y2);
	virtual void DrawRect(int _x1, int _y1, int _x2, int _y2);

	virtual void DrawImage(IImage* image);
	virtual void DrawImage(IImage* image, int x1, int y1, int x2, int y2);
	virtual void DrawImage(IImage* image, int x1, int y1, int x2, int y2, int width, int height);

	virtual IText* CreateTextWithFont(int font);
	virtual IImage* CreateImage(const char* path);

	virtual int GetTextWidth(const char* text, int _font);

	virtual void FreeText(IText* text);
	virtual void FreeImage(IImage* image);

	virtual File* OpenFile(const char* path);

private:
	int mOffsetX;
	int mOffsetY;
	Colour mColour;
};

#endif