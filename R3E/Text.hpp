#ifndef TEXT_HPP
#define TEXT_HPP

#include "OpenGL.hpp"
#include "Vector2.hpp"
#include "Font.hpp"
#include "SmartPointer.hpp"
#include "IText.hpp"

class Text : public IText {
public:
	Text();
	virtual ~Text();

	void SetFont(const SmartPointer<Font>& font);

	virtual void SetText(const char* text);
	virtual void SetPosition(int x, int y);

	virtual int GetWidth();
	virtual int GetHeight();
	virtual const char* GetText();
	virtual int GetCharHeight(char chr);
	virtual int GetCharWidth(char chr);

	virtual void Render(float depth = 1.0f);

private:
	String mText;
	Vector2 mPosition;
	SmartPointer<Font> mFont;
	Font::Vertex* mVertices;
};

#endif