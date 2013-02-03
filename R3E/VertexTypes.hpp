#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include "OpenGL.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

struct UV0_V3F {
	UV0_V3F(){}
	UV0_V3F(Vector2 uv, Vector3 p) : uv0(uv), pos(p) {}

	Vector2 uv0;
	Vector3 pos;
};

struct V3F {
	V3F(){}
	V3F(Vector3 p) : pos(p) {}

	Vector3 pos;

	static void Bind(){
		glVertexPointer(3, GL_FLOAT, sizeof(V3F), 0);
	}
};

struct V3F_UV0 {
	Vector3 pos;
	Vector2 uv0;

	static void Bind(){
		glVertexPointer(3, GL_FLOAT, sizeof(V3F_UV0), 0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0), (void*)sizeof(V3F));
	}
};

struct V3F_UV0_UV1 {
	Vector3 pos;
	Vector2 uv0;
	Vector2 uv1;

	static void Bind(){
		glVertexPointer(3, GL_FLOAT, sizeof(V3F_UV0_UV1), 0);

		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1), (void*)sizeof(V3F));

		OpenGL::ClientActiveTexture(GL_TEXTURE1_ARB);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1), (void*)sizeof(V3F_UV0));

		OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);
	}
};

struct V3F_UV0_UV1_UV2 {
	Vector3 pos;
	Vector2 uv0;
	Vector2 uv1;
	Vector2 uv2;

	static void Bind(){
		glVertexPointer(3, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), 0);

		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), (void*)sizeof(V3F));

		OpenGL::ClientActiveTexture(GL_TEXTURE1_ARB);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), (void*)sizeof(V3F_UV0));

		OpenGL::ClientActiveTexture(GL_TEXTURE2_ARB);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3F_UV0_UV1_UV2), (void*)sizeof(V3F_UV0_UV1));

		OpenGL::ClientActiveTexture(GL_TEXTURE0_ARB);
	}
};

#endif