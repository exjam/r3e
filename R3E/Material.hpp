#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGL.hpp"
#include "Texture.hpp"
#include "SmartPointer.hpp"

class Material {
public:
	Material();
	Material(const char* path);

	~Material();

	void Apply();

	void Default();

	void SetTexture(const char* path);

public:
	short mIs2Side;
	short mIsAlpha;
	short mAlphaTest;
	float mAlphaRef;
	bool mZTest;
	bool mZWrite;
	short mBlendType;
	short mSpecular;

private:
	SmartPointer<Texture> mTexture;
};

#endif