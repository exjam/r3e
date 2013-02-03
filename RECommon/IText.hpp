#ifndef ITEXT_HPP
#define ITEXT_HPP

class IFont;

class IText {
public:
	IText(){}
	virtual ~IText(){}

	virtual void SetPosition(int x, int y) = 0;
	virtual void SetText(const char* text) = 0;

	virtual void Render(float depth = 1.0f) = 0;
	
	virtual const char* GetText() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetCharWidth(char chr) = 0;
	virtual int GetCharHeight(char chr) = 0;

private:
};

#endif
