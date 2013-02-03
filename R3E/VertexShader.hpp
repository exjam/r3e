#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "OpenGL.hpp"
#include "ReadWholeFile.hpp"

class VertexShader {
public:
	VertexShader() : mID(GL_INVALID_INDEX) {}
	VertexShader(const char* path) : mID(GL_INVALID_INDEX) {
		Open(path);
	}

	~VertexShader(){
		OpenGL::DeleteShader(mID);
	}
	
	bool Open(const char* path){
		char* buffer;
		if(ReadWholeFile(buffer, path) == 0) return false;

		GLDEBUG(mID = OpenGL::CreateShader(GL_VERTEX_SHADER_ARB));
		GLDEBUG(OpenGL::ShaderSource(mID, 1, (const GLchar**)&buffer, NULL));
		GLDEBUG(OpenGL::CompileShader(mID));

		delete [] buffer;

		return true;
	}

	GLuint GetID(){
		return mID;
	}

private:
	GLuint mID;
};

#endif
