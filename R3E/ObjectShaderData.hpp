#ifndef OBJECT_SHADER_DATA
#define OBJECT_SHADER_DATA

#include "OpenGL.hpp"

class ObjectShaderData {
public:
	static void LoadFromShader(GLint programObj){
		mLayer1	 = OpenGL::GetUniformLocation(programObj, "layer1");
		mLightmap= OpenGL::GetUniformLocation(programObj, "lightmap");
		mLightmapTrans = OpenGL::GetUniformLocation(programObj, "lightmapTrans");
	}

public:
	static GLint mLayer1;
	static GLint mLightmapTrans;
	static GLint mLightmap;
};

#endif
