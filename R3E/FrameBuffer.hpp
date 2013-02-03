#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "OpenGL.hpp"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void Apply() const;
	void Remove() const;

	void BindTexture() const;
	void BindDepthTexture() const;

	bool Create();

private:
	int mWidth, mHeight;
	GLuint mFrameBuffer, mRenderBuffer, mTexture, mDepthTexture;
};

#endif