#ifndef SHADERPAIR_H
#define SHADERPAIR_H

#include "OpenGL.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

class ShaderPair {
public:
	ShaderPair()
		: mVertexShader(0), mPixelShader(0), mProgramObj(GL_INVALID_INDEX)
	{
	}

	ShaderPair(const char* vtx, const char* pxl)
		: mVertexShader(0), mPixelShader(0), mProgramObj(GL_INVALID_INDEX)
	{
		SetVertexShader(vtx);
		SetPixelShader(pxl);
		Create();
	}

	~ShaderPair(){
		OpenGL::DeleteProgram(mProgramObj);
		SAFE_DELETE(mPixelShader);
		SAFE_DELETE(mVertexShader);
	}

	bool Create(){
		if(!mVertexShader || !mPixelShader) return false;

		mProgramObj = OpenGL::CreateProgram();
		OpenGL::AttachShader(mProgramObj, mVertexShader->GetID());
		OpenGL::AttachShader(mProgramObj, mPixelShader->GetID());
		OpenGL::LinkProgram(mProgramObj);

		return true;
	}

	void SetVertexShader(const char* path){
		SAFE_DELETE(mVertexShader);
		mVertexShader = new VertexShader(path);
	}

	void SetPixelShader(const char* path){
		SAFE_DELETE(mPixelShader);
		mPixelShader = new PixelShader(path);
	}

	void Apply(){
		OpenGL::UseProgram(mProgramObj);
	}

	static void Remove(){
		OpenGL::UseProgram(0);
	}

	GLuint GetProgramObject(){
		return mProgramObj;
	}

private:
	GLuint mProgramObj;
	PixelShader* mPixelShader;
	VertexShader* mVertexShader;
};

#endif
