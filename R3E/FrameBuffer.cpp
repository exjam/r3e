#include "Global.h"

#include "FrameBuffer.hpp"
#include "Log.hpp"

FrameBuffer::FrameBuffer(int width, int height) : mWidth(width), mHeight(height),
	mFrameBuffer(GL_INVALID_INDEX), mRenderBuffer(GL_INVALID_INDEX),
	mTexture(GL_INVALID_INDEX)
{
}

FrameBuffer::~FrameBuffer(){
	if(mTexture != GL_INVALID_INDEX)
		glDeleteTextures(1, &mTexture);

	if(mRenderBuffer != GL_INVALID_INDEX)
		OpenGL::DeleteRenderbuffersEXT(1, &mRenderBuffer);

	if(mFrameBuffer != GL_INVALID_INDEX)
		OpenGL::DeleteFramebuffersEXT(1, &mFrameBuffer);

	mFrameBuffer = mRenderBuffer = mTexture = GL_INVALID_INDEX;
}

void FrameBuffer::Apply() const {
	OpenGL::BindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Remove() const {
	OpenGL::BindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBuffer::BindTexture() const {
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void FrameBuffer::BindDepthTexture() const {
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mDepthTexture);
}

bool FrameBuffer::Create(){
	OpenGL::GenFramebuffersEXT(1, &mFrameBuffer);
	OpenGL::BindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);

	OpenGL::GenRenderbuffersEXT(1, &mRenderBuffer);
	OpenGL::BindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderBuffer);
	OpenGL::RenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mWidth, mHeight);
	OpenGL::FramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mRenderBuffer);

	
	OpenGL::BindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	OpenGL::FramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTexture, 0);

	OpenGL::BindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);

	glGenTextures(1, &mDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,  mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	OpenGL::FramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, mDepthTexture, 0);

	GLenum status = OpenGL::CheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	Remove();
	if(status == GL_FRAMEBUFFER_COMPLETE_EXT) return true;

	LOG("FrameBuffer::Create error: %d", status);
	return false;
}
