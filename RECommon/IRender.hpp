#ifndef IRENDER_HPP
#define IRENDER_HPP

class IText;
class IImage;
class File;
struct Colour;

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef CreateImage
#undef CreateImage
#endif

#ifdef DrawText
#undef DrawText
#endif

class IRender {
public:
	virtual void ResetDrawColour() = 0;
	virtual void SetDrawColour(Colour* colour) = 0;
	virtual void SetDrawColour(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

	virtual void PushTranslate(int x, int y) = 0;
	virtual void PopTranslate(int x, int y) = 0;

	virtual void DrawText(IText* text, float depth = 1.0f) = 0;

	virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
	virtual void DrawRect(int x1, int y1, int x2, int y2) = 0;

	virtual void DrawImage(IImage* image) = 0;
	virtual void DrawImage(IImage* image, int x1, int y1, int x2, int y2) = 0;
	virtual void DrawImage(IImage* image, int x1, int y1, int x2, int y2, int width, int height) = 0;

	virtual int GetTextWidth(const char* text, int font) = 0;

	virtual IText* CreateTextWithFont(int font) = 0;
	virtual IImage* CreateImage(const char* path) = 0;

	virtual void FreeImage(IImage* image) = 0;
	virtual void FreeText(IText* font) = 0;

	virtual File* OpenFile(const char* path) = 0;

private:
};

#endif
