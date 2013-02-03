#ifndef OPENGL_H
#define OPENGL_H

#define IL_STATIC_LIB

#define WIN32_LEAN_AND_MEAN
#define NODRAWTEXT
#define NOTEXTMETRIC
#include <windows.h>
#include <gl/gl.h>
#include <gl/glext.h>
#include <IL/il.h>
#include <IL/ilut.h>

#ifndef GL_INVALID_INDEX
#define GL_INVALID_INDEX 0xFFFFFFFFu
#endif

#include "String.hpp"
#include "FileSystem.hpp"
#include "Math.hpp"

#define OGLEXT(x, y) if(!y) y = (x)wglGetProcAddress(#y);
#define GLDEBUG(x) x; { GLenum e; while((e=glGetError()) != GL_NO_ERROR){ fprintf(stderr, "Error at line number %d, in file %s. glGetError() returned %d for call %s\n",__LINE__, __FILE__, e, #x ); } }

class OpenGL {
private:
	static OpenGL mInstance;

public:
	OpenGL(){}
	~OpenGL(){}

	static void InitGL(){
		OGLEXT(PFNGLCLIENTACTIVETEXTUREPROC, glClientActiveTexture);
		OGLEXT(PFNGLACTIVETEXTUREPROC, glActiveTexture);

		OGLEXT(PFNGLDELETEBUFFERSARBPROC, glDeleteBuffers);
		OGLEXT(PFNGLGENBUFFERSPROC, glGenBuffers);
		OGLEXT(PFNGLBINDBUFFERPROC, glBindBuffer);
		OGLEXT(PFNGLBUFFERDATAPROC, glBufferData);

		OGLEXT(PFNGLATTACHSHADERPROC, glAttachShader);
		OGLEXT(PFNGLCOMPILESHADERPROC, glCompileShader);
		OGLEXT(PFNGLCREATEPROGRAMPROC, glCreateProgram);
		OGLEXT(PFNGLCREATESHADERPROC, glCreateShader);
		OGLEXT(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
		OGLEXT(PFNGLDELETESHADERPROC, glDeleteShader);
		OGLEXT(PFNGLLINKPROGRAMPROC, glLinkProgram);
		OGLEXT(PFNGLSHADERSOURCEPROC, glShaderSource);
		OGLEXT(PFNGLUSEPROGRAMPROC, glUseProgram);
		OGLEXT(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
		OGLEXT(PFNGLVERTEXATTRIB4FVARBPROC, glVertexAttrib4fvARB);
		OGLEXT(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
		OGLEXT(PFNGLUNIFORMMATRIX4FVARBPROC, glUniformMatrix4fvARB);
		OGLEXT(PFNGLUNIFORM3FPROC, glUniform3f);
		OGLEXT(PFNGLUNIFORM1IPROC, glUniform1i);

		OGLEXT(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
		OGLEXT(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
		OGLEXT(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
		
		OGLEXT(PFNGLGENPROGRAMSARBPROC, glGenProgramsARB);
		OGLEXT(PFNGLPROGRAMSTRINGARBPROC, glProgramStringARB);
		OGLEXT(PFNGLBINDPROGRAMARBPROC, glBindProgramARB);
		OGLEXT(PFNGLDELETEPROGRAMSARBPROC, glDeleteProgramARB);
		OGLEXT(PFNGLPROGRAMENVPARAMETER4FARBPROC, glProgramEnvParameter4fARB);
		OGLEXT(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);

		OGLEXT(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
		OGLEXT(PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT);
		OGLEXT(PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT);
		OGLEXT(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
		OGLEXT(PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT);
		OGLEXT(PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);
		OGLEXT(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
		OGLEXT(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT);
		OGLEXT(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, glFramebufferTexture2DEXT);
		OGLEXT(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);

		ClientActiveTexture(GL_TEXTURE0_ARB);

		glShadeModel(GL_FLAT);
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CullFace(true);
		AlphaTest(false);
		DepthTest(false);
		DepthWrite(false);
		Blend(false);
	}

	static void Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){
	   GLdouble xmin, xmax, ymin, ymax;

	   ymax = zNear * tan(fovy * M_PI / 360.0);
	   ymin = -ymax;
	   xmin = ymin * aspect;
	   xmax = ymax * aspect;

	   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	}

	static void FramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){
		glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
	}

	static GLenum CheckFramebufferStatusEXT(GLenum target){
		return glCheckFramebufferStatusEXT(target);
	}

	static void BindFramebufferEXT(GLenum target, GLuint framebuffer){
		glBindFramebufferEXT(target, framebuffer);
	}

	static void DeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers){
		glDeleteFramebuffersEXT(n, framebuffers);
	}

	static void GenFramebuffersEXT(GLsizei n, GLuint *framebuffers){
		glGenFramebuffersEXT(n, framebuffers);
	}

	static void BindRenderbufferEXT(GLenum target, GLuint renderbuffer){
		glBindRenderbufferEXT(target, renderbuffer);
	}

	static void DeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers){
		glDeleteRenderbuffersEXT(n, renderbuffers);
	}

	static void GenRenderbuffersEXT(GLsizei n, GLuint *renderbuffers){
		glGenRenderbuffersEXT(n, renderbuffers);
	}

	static void RenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height){
		glRenderbufferStorageEXT(target, internalformat, width, height);
	}

	static void FramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){
		glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
	}

	
	static void ProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
		glProgramLocalParameter4fARB(target, index, x, y, z, w);
	}

	static void ProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
		glProgramEnvParameter4fARB(target, index, x, y, z, w);
	}

	static void ProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid *string){
		glProgramStringARB(target, format, len, string);
	}

	static void BindProgramARB(GLenum target, GLuint program){
		glBindProgramARB(target, program);
	}

	static void DeleteProgramARB(GLsizei n, const GLuint *programs){
		glDeleteProgramARB(n, programs);
	}

	static void GenProgramsARB(GLsizei n, GLuint* programs){
		glGenProgramsARB(n, programs);
	}

	static void Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
		glUniform3f(location, v0, v1, v2);
	}

	static void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer){
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	static void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
		glUniformMatrix4fvARB(location, count, transpose, value);
	}

	static GLint GetAttribLocation(GLhandleARB programObj, const GLcharARB *name){
		return glGetAttribLocation(programObj, name);
	}

	static void VertexAttrib4fv(GLuint index, const GLfloat *v){
		glVertexAttrib4fvARB(index, v);
	}

	static void BindBuffer(GLenum target, GLuint buffer){
		glBindBuffer(target, buffer);
	}

	static void BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage){
		glBufferData(target, size, data, usage);
	}

	static void GenBuffers(GLsizei n, GLuint *buffers){
		glGenBuffers(n, buffers);
	}

	static void ClientActiveTexture(GLenum target){
		glClientActiveTexture(target);
	}

	static void ActiveTexture(GLenum target){
		glActiveTexture(target);
	}

	static void DeleteBuffers(GLsizei n, const GLuint *buffers){
		glDeleteBuffers(n, buffers);
	}

	static GLint GetUniformLocation(GLuint program, const GLchar *name){
		return glGetUniformLocation(program, name);
	}

	static void AttachShader(GLuint program, GLuint shader){
		glAttachShader(program, shader);
	}

	static void CompileShader(GLuint shader){
		glCompileShader(shader);
	}

	static GLuint CreateProgram(){
		return glCreateProgram();
	}

	static GLuint CreateShader(GLenum type){
		return glCreateShader(type);
	}

	static void DeleteProgram(GLuint& program){
		if(program == GL_INVALID_INDEX) return;
		glDeleteProgram(program);
		program = GL_INVALID_INDEX;
	}

	static void DeleteShader(GLuint& shader){
		if(shader == GL_INVALID_INDEX) return;
		glDeleteShader(shader);
		shader = GL_INVALID_INDEX;
	}

	static void LinkProgram(GLuint program){
		glLinkProgram(program);
	}

	static void ShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint *length){
		glShaderSource(shader, count, string, length);
	}

	static void UseProgram(GLuint program){
		glUseProgram(program);
	}

	static void UnloadBuffer(GLuint& buf){
		if(buf == GL_INVALID_INDEX) return;
		glDeleteBuffers(1, &buf);
		buf = GL_INVALID_INDEX;
	}

	static void EnableVertexAttribArray(GLuint index){
		glEnableVertexAttribArray(index);
	}

	static void DisableVertexAttribArray(GLuint index){
		glDisableVertexAttribArray(index);
	}

	static void Uniform1i(GLint location, GLint v0){
		glUniform1i(location, v0);
	}

	static void CullFace(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	static void AlphaTest(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_ALPHA_TEST);
		else glDisable(GL_ALPHA_TEST);
	}

	static void DepthTest(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	static void DepthWrite(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glDepthMask(GL_TRUE);
		else glDepthMask(GL_FALSE);
	}

	static void Blend(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_BLEND);
		else glDisable(GL_BLEND);
	}

	static void BlendFunc(GLenum src, GLenum dest){
		static GLenum sState = GL_SRC_ALPHA;
		static GLenum dState = GL_ONE_MINUS_SRC_ALPHA;
		if(sState == src && dState == dest) return;
		sState = src;
		dState = dest;
		glBlendFunc(src, dest);
	}

	static void VertexShaderARB(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_VERTEX_PROGRAM_ARB);
		else glDisable(GL_VERTEX_PROGRAM_ARB);
	}

	static void PixelShaderARB(bool enable){
		static bool state = false;
		if(state == enable) return;
		state = enable;
		if(enable) glEnable(GL_FRAGMENT_PROGRAM_ARB);
		else glDisable(GL_FRAGMENT_PROGRAM_ARB);
	}

public:
	static unsigned int mVertexCount;

private:
	static PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
	static PFNGLACTIVETEXTUREPROC glActiveTexture;

	static PFNGLGENBUFFERSPROC glGenBuffers;
	static PFNGLBINDBUFFERPROC glBindBuffer;
	static PFNGLBUFFERDATAPROC glBufferData;
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;

	static PFNGLATTACHSHADERPROC glAttachShader;
	static PFNGLCOMPILESHADERPROC glCompileShader;
	static PFNGLCREATEPROGRAMPROC glCreateProgram;
	static PFNGLCREATESHADERPROC glCreateShader;
	static PFNGLDELETEPROGRAMPROC glDeleteProgram;
	static PFNGLDELETESHADERPROC glDeleteShader;
	static PFNGLLINKPROGRAMPROC glLinkProgram;
	static PFNGLSHADERSOURCEPROC glShaderSource;
	static PFNGLUSEPROGRAMPROC glUseProgram;
	static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	static PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
	static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	static PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
	static PFNGLUNIFORM1IPROC glUniform1i;
	static PFNGLUNIFORM3FPROC glUniform3f;

	static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

	static PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
	static PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
	static PFNGLBINDPROGRAMARBPROC glBindProgramARB;
	static PFNGLDELETEPROGRAMSARBPROC glDeleteProgramARB;
	static PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
	static PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
	
	static PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
	static PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
	static PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
	static PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
	static PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
	static PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
	static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
	static PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
	static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
	static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
};

#endif
