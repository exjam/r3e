#include "Global.h"

#include "Log.hpp"
#include "Font.hpp"
#include "OpenGL.hpp"
#include "FileSystem.hpp"
#include "SmartPointer.hpp"
#include "ScopedPointer.hpp"
#include "ResourceManager.hpp"

Font::Font(){}
Font::~Font(){}

void Font::Unload(){
	mTexture = SmartPointer<Texture>();
}

bool Font::Load(const char* path){
	ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
	if(!fh) return false;

	if(fh->Read<char>() != 'B') return false;
	if(fh->Read<char>() != 'M') return false;
	if(fh->Read<char>() != 'F') return false;
	if(fh->Read<char>() !=   3) return false;

	while(!fh->IsEOF()){
		char blockType = fh->Read<char>();
		if(fh->IsEOF()) break;
		unsigned int size = fh->Read<unsigned int>();
		switch(blockType){
			case BLOCK_COMMON:
				fh->Skip(4);
				mWidth = float(fh->Read<short>());
				mHeight = float(fh->Read<short>());
				fh->Skip(size - 8);
			break;
			case BLOCK_PAGES:
			{
				String tex = fh->ReadTerminatedString();
				fh->Skip(size - (tex.Length() + 1));
				mTexture = TEX_MGR().Load(String("R3EFonts\\%1").arg(tex));
			}
			break;
			case BLOCK_CHARACTERS:
			{
				unsigned int count = size / 20;
				for(unsigned int i = 0; i < count; ++i){
					unsigned int id = fh->Read<unsigned int>();
					Character& chr = mCharacters[id];
					chr.mID = id;
					fh->ReadData(4 + (char*)&chr, 16);
				}
			}
			break;
			default:
				fh->Skip(size);
		}
	}

	fh->Close();
	return true;
}

void Font::Bind() const {
	glBindTexture(GL_TEXTURE_2D, mTexture->GetTextureID());
}

int Font::GetCharHeight(char chr) const {
	return mCharacters[chr].mHeight + mCharacters[chr].mYOffset;
}

int Font::GetCharWidth(char chr) const {
	return mCharacters[chr].mXAdvance;
}

int Font::GetStringWidth(const char* str) const {
	int width = 0;
	while(*str){
		width += *str;
		++str;
	}

	return width;
}

void Font::GenerateVertices(Vertex*& verts, const char* text) const {
	if(verts) delete [] verts;

	float curX = 0;
	int len = strlen(text);
	verts = new Vertex[len*4];
	Vertex* curVert = &verts[0];

	for(int i = 0; i < len; ++i){
		char chr = text[i];
		float chrX = mCharacters[chr].mX;
		float chrY = mCharacters[chr].mY;
		float chrW = mCharacters[chr].mWidth;
		float chrH = mCharacters[chr].mHeight;
		float chrOffX = mCharacters[chr].mXOffset;
		float chrOffY = mCharacters[chr].mYOffset;

		//Upper Left
		curVert->u = chrX / mWidth;
		curVert->v = chrY / mHeight;
		curVert->x = curX + chrOffX;
		curVert->y = chrOffY;
		curVert->z = 0.0f;
		++curVert;

		//Lower Left
		curVert->u = chrX / mWidth;
		curVert->v = (chrY + chrH) / mHeight;
		curVert->x = curX + chrOffX;
		curVert->y = chrH + chrOffY;
		curVert->z = 0.0f;
		++curVert;

		//Lower Right
		curVert->u = (chrX + chrW) / mWidth;
		curVert->v = (chrY + chrH) / mHeight;
		curVert->x = chrW + curX + chrOffX;
		curVert->y = chrH + chrOffY;
		curVert->z = 0.0f;
		++curVert;

		//Upper Right
		curVert->u = (chrX + chrW) / mWidth;
		curVert->v = chrY / mHeight;
		curVert->x = chrW + curX + chrOffX;
		curVert->y = chrOffY;
		curVert->z = 0.0f;
		++curVert;

		curX += float(mCharacters[chr].mXAdvance);
	}
}
