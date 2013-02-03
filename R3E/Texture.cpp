#include "Global.h"

#include "Log.hpp"
#include "Texture.hpp"

Texture::Texture() : mTexID(GL_INVALID_INDEX) {}
Texture::~Texture(){}

void Texture::Unload(){
	if(mTexID == GL_INVALID_INDEX) return;
	glDeleteTextures(1, &mTexID);
	mTexID = GL_INVALID_INDEX;
}

bool Texture::Load(const char* pathV){
	static bool hasInitIL = false;
	unsigned char* data = 0;
	if(!hasInitIL){
		ilInit();
		ilutRenderer(ILUT_OPENGL);
		hasInitIL = true;
	}

	ILuint ilTex;
	ilGenImages(1, &ilTex);
	ilBindImage(ilTex);
	File* file = FILE_SYS()->OpenFile(pathV, "rb");
	if(!file) goto loadFail;
	long size = file->Size();
	data = new unsigned char[size];
	file->ReadData(data, size);
	file->Close();
	delete file;

	if(!ilLoadL(IL_TYPE_UNKNOWN, data, size)) goto loadFail;

	delete [] data;

	mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	iluFlipImage();

	mTexID = ilutGLBindTexImage();
	ilDeleteImages(1, &ilTex);

	LOG("Loaded: '%s' W: %u H: %u", pathV, mWidth, mHeight);

	return true;

loadFail:
	LOG("Error: Could not load texure '%s'", pathV);
	if(data) delete [] data;
	return false;
}

GLuint Texture::GetTextureID() const {
	return mTexID;
}

int Texture::GetWidth() const {
	return mWidth;
}

int Texture::GetHeight() const {
	return mHeight;
}
