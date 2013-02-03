#ifndef MULTISAMPLE_HPP
#define MULTISAMPLE_HPP

#include "OpenGL.hpp"

// WGL_ARB_multisample

#define WGL_SAMPLE_BUFFERS_ARB             0x2041
#define WGL_SAMPLES_ARB                    0x2042

// WGL_ARB_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_ARB       0x2000
#define WGL_DRAW_TO_WINDOW_ARB             0x2001
#define WGL_DRAW_TO_BITMAP_ARB             0x2002
#define WGL_ACCELERATION_ARB               0x2003
#define WGL_NEED_PALETTE_ARB               0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB        0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB         0x2006
#define WGL_SWAP_METHOD_ARB                0x2007
#define WGL_NUMBER_OVERLAYS_ARB            0x2008
#define WGL_NUMBER_UNDERLAYS_ARB           0x2009
#define WGL_TRANSPARENT_ARB                0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB    0x203B
#define WGL_SHARE_DEPTH_ARB                0x200C
#define WGL_SHARE_STENCIL_ARB              0x200D
#define WGL_SHARE_ACCUM_ARB                0x200E
#define WGL_SUPPORT_GDI_ARB                0x200F
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_STEREO_ARB                     0x2012
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_RED_BITS_ARB                   0x2015
#define WGL_RED_SHIFT_ARB                  0x2016
#define WGL_GREEN_BITS_ARB                 0x2017
#define WGL_GREEN_SHIFT_ARB                0x2018
#define WGL_BLUE_BITS_ARB                  0x2019
#define WGL_BLUE_SHIFT_ARB                 0x201A
#define WGL_ALPHA_BITS_ARB                 0x201B
#define WGL_ALPHA_SHIFT_ARB                0x201C
#define WGL_ACCUM_BITS_ARB                 0x201D
#define WGL_ACCUM_RED_BITS_ARB             0x201E
#define WGL_ACCUM_GREEN_BITS_ARB           0x201F
#define WGL_ACCUM_BLUE_BITS_ARB            0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB           0x2021
#define WGL_DEPTH_BITS_ARB                 0x2022
#define WGL_STENCIL_BITS_ARB               0x2023
#define WGL_AUX_BUFFERS_ARB                0x2024
#define WGL_NO_ACCELERATION_ARB            0x2025
#define WGL_GENERIC_ACCELERATION_ARB       0x2026
#define WGL_FULL_ACCELERATION_ARB          0x2027
#define WGL_SWAP_EXCHANGE_ARB              0x2028
#define WGL_SWAP_COPY_ARB                  0x2029
#define WGL_SWAP_UNDEFINED_ARB             0x202A
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_TYPE_COLORINDEX_ARB            0x202C

#include <strsafe.h>
class Multisample {
public:
	Multisample(){}
	~Multisample(){}

	static bool SetAA(int samples, int &pf){
		Multisample ms;

		if(!ms.CreateDummyWindow()){
			ms.DestroyDummyWindow();
			return false;
		}
		
		bool result = ms.ChooseMultisamplePF(pf, samples);
		ms.DestroyDummyWindow();

		return result;
	}

	static int SetBestAA(int &pf){
		Multisample ms;

		if(!ms.CreateDummyWindow()){
			ms.DestroyDummyWindow();
			return false;
		}
		
		int samples = ms.ChooseBestMultisamplePF(pf);

		ms.DestroyDummyWindow();

		return samples;
	}

private:
    int ChooseBestMultisamplePF(int &pf){
        int attributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     24,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     16,
            WGL_STENCIL_BITS_ARB,   0,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB,        0,
            0, 0
        };

        int returnedPixelFormat = 0;
        UINT numFormats = 0;
        BOOL bStatus = FALSE;
        for(int samples = 16; samples > 0; samples /= 2){
            attributes[17] = samples;

            bStatus = wglChoosePixelFormatARB(mHDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);
            if(bStatus == FALSE || !numFormats) continue;

            pf = returnedPixelFormat;
            return samples;
        }

		return 0;
    }

	bool ChooseMultisamplePF(int &pf, int samples){
		int attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     24,
			WGL_ALPHA_BITS_ARB,     8,
			WGL_DEPTH_BITS_ARB,     16,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        samples,
			0, 0
		};

        int returnedPixelFormat = 0;
        UINT numFormats = 0;
        BOOL bStatus = wglChoosePixelFormatARB(mHDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);
        if(bStatus == TRUE && numFormats){
            pf = returnedPixelFormat;
            return true;
        }else
			return false;
	}

	bool CreateDummyWindow(){
        mWndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        mWndClass.lpfnWndProc = DummyGLWndProc;
        mWndClass.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
        mWndClass.lpszClassName = "MSTEST";
		mWndClass.cbClsExtra = 0;
		mWndClass.cbWndExtra = 0;
		mWndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		mWndClass.hbrBackground = NULL;
		mWndClass.lpszMenuName = NULL;

        if(!RegisterClass(&mWndClass))
            return false;

        mHwnd = CreateWindow(mWndClass.lpszClassName, "", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, mWndClass.hInstance, 0);

        if(!mHwnd)
            return false;

        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pf = ChoosePixelFormat(mHDC, &pfd);
        if(!SetPixelFormat(mHDC, pf, &pfd))
            return false;

        if(!(mHRC = wglCreateContext(mHDC)))
            return false;

        if(!wglMakeCurrent(mHDC, mHRC))
            return false;

        return true;
	}

	void DestroyDummyWindow(){
        if(mHwnd){
            PostMessage(mHwnd, WM_CLOSE, 0, 0);

            BOOL bRet;
            MSG msg;
            while((bRet = GetMessage(&msg, 0, 0, 0)) != 0){ 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            }
        }        

        UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
	}

	static LRESULT CALLBACK DummyGLWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        switch(msg){
        case WM_CREATE:
			if(!(mHDC = GetDC(mHwnd)))
                return -1;
            break;
        case WM_DESTROY:
            if(mHDC){
                if(mHRC){
                    wglMakeCurrent(mHDC, 0);
                    wglDeleteContext(mHRC);
                    mHRC = 0;
                }

                ReleaseDC(mHwnd, mHDC);
                mHDC = 0;
            }

            PostQuitMessage(0);
            return 0;
        default:
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

	BOOL wglChoosePixelFormatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats){
		typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
		static PFNWGLCHOOSEPIXELFORMATARBPROC pfnChoosePixelFormatARB = 0;

		if(!pfnChoosePixelFormatARB)
			pfnChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

		return pfnChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
	}

private:
    WNDCLASS mWndClass;
	static HWND mHwnd;
	static HGLRC mHRC;
	static HDC mHDC;
};

#endif
