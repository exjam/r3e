#include "Global.h"

#include "Material.hpp"
#include "ResourceManager.hpp"

Material::Material(){
	Default();
}

Material::Material(const char* path){
	SetTexture(path);
	Default();
}

Material::~Material(){}

void Material::Default(){
	mIs2Side = 0;
	mAlphaTest = 1;
	mAlphaRef = 0.5f;
	mZTest = true;
	mZWrite = true;
	mIsAlpha = 0;
	mBlendType = 0;
	mSpecular = 0;
}

void Material::Apply(){
	OpenGL::CullFace(!mIs2Side);

	if(mAlphaTest && !mSpecular){
		OpenGL::AlphaTest(true);
		glAlphaFunc(GL_GEQUAL, mAlphaRef);
	}else{
		OpenGL::AlphaTest(false);
	}

	OpenGL::DepthTest(mZTest);
	OpenGL::DepthWrite(mZTest);

	if(mIsAlpha && !mSpecular){
		OpenGL::Blend(true);

		if(mBlendType == 0){
			OpenGL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}else if(mBlendType == 1){
			OpenGL::BlendFunc(GL_ONE, GL_ONE);
		}
	}else{
		OpenGL::Blend(false);
	}

	glBindTexture(GL_TEXTURE_2D, mTexture->GetTextureID());
}

void Material::SetTexture(const char* path){
	mTexture = TEX_MGR().Load(path);
}
