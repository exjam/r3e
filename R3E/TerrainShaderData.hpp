#ifndef TERRAIN_SHADER_DATA
#define TERRAIN_SHADER_DATA

#include "OpenGL.hpp"

class TerrainShaderData {
public:
	static void LoadFromShader(GLint programObj){
		mLayer1	 = OpenGL::GetUniformLocation(programObj, "layer1");
		mLayer2  = OpenGL::GetUniformLocation(programObj, "layer2");
		mLightmap= OpenGL::GetUniformLocation(programObj, "lightmap");
	}

public:
	static GLint mLayer1;
	static GLint mLayer2;
	static GLint mLightmap;
};

#endif
