#include "Global.h"

#include "Window.hpp"
#include "StateManager.hpp"
#include "..\RGE\TranslateWin32Event.hpp"
#include "..\R3E\SceneManager.hpp"

Window::Window()
	: mFrameTime(1000 / 60)
{
}

Window::~Window(){
}

void Window::ResizeView(int width, int height){
	gScene->ResizeScene(width, height);

	mWidth = width;
	mHeight = height;
}

int Window::Proc(UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_ACTIVATE:
			SetActive(!HIWORD(wParam));
			break;

		case WM_SIZE:
			ResizeView(LOWORD(lParam), HIWORD(lParam));
			break;
	}

	GuiEvent* evt = TranslateWin32Event(uMsg, wParam, lParam);
	if(evt && gActiveState)
		gActiveState->HandleEvent(evt);

	return 0;
}

void Window::SwapBuffers(){
	//add frame sleeping stuff here
	static unsigned long long frequency = 0;
	static unsigned long long start_time = 0;
	static unsigned long long next_frame = 0;
	static unsigned int frame_count = 0;
	static unsigned int vcount = 0;

	if(frequency == 0){
		QueryPerformanceFrequency((PLARGE_INTEGER)&frequency);
		QueryPerformanceCounter((PLARGE_INTEGER)&start_time);
		next_frame = start_time;
	}

	::SwapBuffers(mHDC);

	++vcount;
	unsigned long long cur_time;
	QueryPerformanceCounter((PLARGE_INTEGER)&cur_time);

	bool didSleep = false;
	while(cur_time < next_frame){
		if(!didSleep){
			unsigned long long time_left = (next_frame - cur_time) * 1000 / frequency;
			if(time_left >= 2){
				time_left--;
				Sleep((DWORD)time_left);
				didSleep = true;
			}
		}

		QueryPerformanceCounter((PLARGE_INTEGER)&cur_time);
	}

	next_frame += frequency / 60;
}

int Window::Run(){
	MSG msg;
	msg.wParam = 0;

	mRunning = TRUE;
	
	while(mRunning && GetMessage(&msg, 0, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	mRunning = FALSE;
	return msg.wParam;
}

int Window::Create(){
	int PixelFormat;
	WNDCLASS wc;
	RECT WindowRect;
	WindowRect.left = WindowRect.top = 0;
	WindowRect.right = mWidth;
	WindowRect.bottom = mHeight;

	mInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "R3EWND";

	if(!RegisterClass(&wc)) return FALSE;

	AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	mHWND = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE /*0*/, "R3EWND", mTitle, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* WS_POPUP*/, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, mInstance, (LPVOID)this);

	if(!mHWND){
		Destroy();
		return FALSE;
	}

	static	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE, 0,0, 0, 0
	};

	mHDC = GetDC(mHWND);
	if(!mHDC){
		Destroy();
		return FALSE;
	}

	PixelFormat = 0;
	int samples = Multisample::SetBestAA(PixelFormat);
	printf("Set to %u samples!\n", samples);

	if(!PixelFormat){
		PixelFormat = ChoosePixelFormat(mHDC, &pfd);
		if(!PixelFormat){
			Destroy();
			return FALSE;
		}
	}

	if(!SetPixelFormat(mHDC, PixelFormat, &pfd)){
		Destroy();
		return FALSE;
	}

	mHRC = wglCreateContext(mHDC);

	if(!mHRC){
		Destroy();
		return FALSE;
	}

	ShowWindow(mHWND, SW_SHOW);
	SetForegroundWindow(mHWND);
	SetFocus(mHWND);
	ResizeView(mWidth, mHeight);

	mActive = TRUE;

	return TRUE;
}

void Window::SetOpenGLThread(){
	wglMakeCurrent(mHDC, mHRC);
}

void Window::Destroy(){
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(mHRC);
	ReleaseDC(mHWND, mHDC);
	DestroyWindow(mHWND);

	UnregisterClass("R3EWND", mInstance);

	mHWND = NULL;
	mHRC = NULL;
	mHDC = NULL;
}

void Window::Close(){
	mRunning = FALSE;
}

void Window::SetActive(BOOL active){
	mActive = active;
}

void Window::SetSize(int width, int height){
	mWidth = width;
	mHeight = height;

	if(mHWND)
		SetWindowPos(mHWND, 0, 0, 0, mWidth, mHeight, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetTitle(const char* title){
	mTitle = title;

	if(mHWND)
		SetWindowText(mHWND, title);
}

int Window::Height() const {
	return mHeight;
}

int Window::Width() const {
	return mWidth;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	static Window* wnd = NULL;
	if(uMsg == WM_NCCREATE)
		wnd = (Window*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
	else if(uMsg == WM_CLOSE || uMsg == WM_QUIT || uMsg == WM_DESTROY){
		wnd->mRunning = FALSE;
		return 0;
	}

	if(wnd) wnd->Proc(uMsg, wParam, lParam);

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
