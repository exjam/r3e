#ifndef FONT_H
#define FONT_H

#include "Texture.hpp"
#include "Resource.hpp"
#include "SmartPointer.hpp"

class Font : public Resource {
private:
	struct Character {
		int mID;
		short mX;
		short mY;
		short mWidth;
		short mHeight;
		short mXOffset;
		short mYOffset;
		short mXAdvance;
		char mPage;
		char mChannel;
	};

public:
	struct Vertex {
		float u;
		float v;
		float x;
		float y;
		float z;
	};

private:
	enum {
		BLOCK_INVALID,
		BLOCK_INFO,
		BLOCK_COMMON,
		BLOCK_PAGES,
		BLOCK_CHARACTERS,
		BLOCK_KERNING,
	};

public:
	Font();
	virtual ~Font();

	virtual void Unload();

	virtual bool Load(const char* path);

	void Bind() const;

	void GenerateVertices(Vertex*& verts, const char* text) const;

	int GetCharHeight(char chr) const;
	int GetCharWidth(char chr) const;
	int GetStringWidth(const char* str) const;

private:
	SmartPointer<Texture> mTexture;

	float mWidth, mHeight;
	Character mCharacters[256];
};

#endif