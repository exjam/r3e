#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGL.hpp"
#include "Resource.hpp"

class Texture : public Resource {
public:
	Texture();
	virtual ~Texture();

	virtual bool Load(const char* pathV);
	virtual void Unload();

	int GetWidth() const;
	int GetHeight() const;
	GLuint GetTextureID() const;

private:
	int mWidth;
	int mHeight;
	GLuint mTexID;
};

#endif