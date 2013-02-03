#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "Windows.hpp"
#include <time.h>
#include <gl/gl.h>
#include "String.hpp"

class OpenGLWindow {
public:
	OpenGLWindow() : mRunning(TRUE), mClassName("ROSEGL"), mFrameSpeed(15) {
		memset(mKeys, 0, 256);
	}

	virtual ~OpenGLWindow(){}

	virtual BOOL InitGL() = 0;
	virtual void DrawGL() = 0;
	virtual void ResizeGL(int width, int height) = 0;

	virtual void MouseWheel(WPARAM, LPARAM){}
	virtual void MouseMove(WPARAM, LPARAM){}
	virtual void MouseLButtonUp(WPARAM, LPARAM){}
	virtual void KeyDown(WPARAM, LPARAM){}
	virtual void KeyUp(WPARAM, LPARAM){}

	virtual int Proc(UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
			case WM_ACTIVATE:
				SetActive(!HIWORD(wParam));
				return 0;

			case WM_MOUSEMOVE:
				{
					static clock_t lastMM = clock();
					if(clock() - lastMM > mFrameSpeed){
						MouseMove(wParam, lParam);
						lastMM = clock();
					}
				}
				return 0;

			case WM_LBUTTONUP:
				MouseLButtonUp(wParam, lParam);
				return 0;

			case WM_MOUSEWHEEL:
				MouseWheel(wParam, lParam);
				return 0;

			case WM_KEYDOWN:
				SetKey(wParam, TRUE);
				KeyDown(wParam, lParam);
				return 0;

			case WM_KEYUP:
				SetKey(wParam, FALSE);
				KeyUp(wParam, lParam);
				return 0;

			case WM_SIZE:
				ResizeGL(LOWORD(lParam), HIWORD(lParam));
				return 0;
		};

		return 0;
	}

	int Run(){
		MSG msg;
		msg.wParam = 0;

		clock_t lastDraw = clock();
		while(mRunning){
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				if(msg.message == WM_QUIT){
					mRunning = FALSE;
				}else{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}else{
				if(!mActive) continue;
				if(clock() - lastDraw > mFrameSpeed){
					lastDraw = clock();
					DrawGL();
					SwapBuffers(mHDC);
				}
			}
		}

		Destroy();

		return msg.wParam;
	}

	int Create(){
		GLuint PixelFormat;
		WNDCLASS wc;
		RECT WindowRect;
		WindowRect.left = WindowRect.top = 0;
		WindowRect.right = mWidth;
		WindowRect.bottom = mHeight;

		mInstance = GetModuleHandle(NULL);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = mClassName;

		if(!RegisterClass(&wc)) return FALSE;

		AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		mHWND = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE /*0*/, mClassName, mTitle, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* WS_POPUP*/, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, mInstance, (LPVOID)this);

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

		PixelFormat = ChoosePixelFormat(mHDC, &pfd);
		if(!PixelFormat){
			Destroy();
			return FALSE;
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

		if(!wglMakeCurrent(mHDC, mHRC)){
			Destroy();
			return FALSE;
		}

		ShowWindow(mHWND, SW_SHOW);
		SetForegroundWindow(mHWND);
		SetFocus(mHWND);
		ResizeGL(mWidth, mHeight);

		if(!InitGL()){
			Destroy();
			return FALSE;
		}

		mActive = TRUE;

		return TRUE;
	}

	void Destroy(){
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mHRC);
		ReleaseDC(mHWND, mHDC);
		DestroyWindow(mHWND);

		UnregisterClass(mClassName, mInstance);

		mHWND = NULL;
		mHRC = NULL;
		mHDC = NULL;
	}

	void SetActive(BOOL active){
		mActive = active;
	}

	void SetKey(int key, bool value){
		mKeys[key] = value;
	}

	OpenGLWindow& SetClassName(const char* classname){
		mClassName = classname;
		return *this;
	}

	OpenGLWindow& SetSize(int width, int height){
		mWidth = width;
		mHeight = height;
		if(mHWND){
			SetWindowPos(mHWND, 0, 0, 0, mWidth, mHeight, SWP_NOMOVE | SWP_NOZORDER);
		}
		return *this;
	}

	OpenGLWindow& SetTitle(const char* title){
		mTitle = title;
		if(mHWND){
			SetWindowText(mHWND, title);
		}

		return *this;
	}

	BOOL Key(int key){
		return mKeys[key];
	}

	int Height(){
		return mHeight;
	}

	int Width(){
		return mWidth;
	}

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		static OpenGLWindow* wnd = NULL;
		if(uMsg == WM_NCCREATE)
			wnd = (OpenGLWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		else if(uMsg == WM_CLOSE)
			PostQuitMessage(0);

		if(wnd) wnd->Proc(uMsg, wParam, lParam);

		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}

protected:
	int mFrameSpeed;

	String mTitle;
	String mClassName;
	int mWidth;
	int mHeight;

	HWND mHWND;
	HDC mHDC;
	HGLRC mHRC;
	HINSTANCE mInstance;

	unsigned char mKeys[256];
	BOOL mActive;
	BOOL mRunning;
};

#endif