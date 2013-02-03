#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include "Windows.hpp"

#include <time.h>
#include <gl/gl.h>
#include "String.hpp"
#include "../R3E/Multisample.hpp"

extern class Window* gWindow;

class Window {
public:
	Window();
	~Window();

	int Run();
	int Create();
	int Proc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Close();
	void Destroy();

	void SwapBuffers();
	void SetOpenGLThread();
	void SetActive(BOOL active);
	void SetTitle(const char* title);
	void SetSize(int width, int height);

	void ResizeView(int width, int height);

	int Width() const;
	int Height() const;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	int mFrameTime;

	String mTitle;
	int mWidth;
	int mHeight;

	HWND mHWND;
	HDC mHDC;
	HGLRC mHRC;
	HINSTANCE mInstance;

	BOOL mActive;
	BOOL mRunning;
};

#endif