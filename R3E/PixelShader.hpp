#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include "OpenGL.hpp"
#include "ReadWholeFile.hpp"

class PixelShader {
public:
	PixelShader() : mID(GL_INVALID_INDEX) {}
	PixelShader(const char* path) : mID(GL_INVALID_INDEX) {
		Open(path);
	}

	~PixelShader(){
		OpenGL::DeleteShader(mID);
	}

	bool Open(const char* path){
		char* buffer;
		if(ReadWholeFile(buffer, path) == 0) return false;

		GLDEBUG(mID = OpenGL::CreateShader(GL_FRAGMENT_SHADER_ARB));
		GLDEBUG(OpenGL::ShaderSource(mID, 1, (const GLchar**)&buffer, NULL));
		GLDEBUG(OpenGL::CompileShader(mID));

		delete [] buffer;

		return true;
	}

	inline GLuint GetID() const {
		return mID;
	}

private:
	GLuint mID;
};

#endif
