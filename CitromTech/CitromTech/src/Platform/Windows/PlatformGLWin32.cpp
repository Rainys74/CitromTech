#include "Platform/PlatformGL.h"

#ifdef CT_PLATFORM_WINDOWS
#include "WindowsIncludes.h"

#include "CitromAssert.h"
#if 0
//#include <GL/glew.h>
//#include <GL/wglew.h>

namespace Citrom::Platform::GL
{
	struct ContextGLData
	{
		// OpenGL
		HDC DC; // Device Context
		HGLRC RC; // Render Context
		int PF; // Pixel Format
	};

	ContextGL::ContextGL(Window* window)
		: m_Window(window)
	{
		m_Internal = CTL::CreateScopedPtr<ContextGLData>();
	}
	ContextGL::~ContextGL()
	{
		if (m_Internal->DC)
			ReleaseDC(static_cast<HWND>(m_Window->Win32TryGetHWnd()), m_Internal->DC);
		if (m_Internal->RC)
			wglDeleteContext(m_Internal->RC);
	}

	void ContextGL::Create()
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);									// size of this pfd
		pfd.nVersion = 1;															// version number
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	// support window, OpenGL, double buffered.
		pfd.iPixelType = PFD_TYPE_RGBA;												// RGBA type
		pfd.cColorBits = 24;														// 24-bit color depth
		pfd.cRedBits = 0;															// color bits ignored
		pfd.cRedShift = 0;
		pfd.cGreenBits = 0;
		pfd.cGreenShift = 0;
		pfd.cBlueBits = 0;
		pfd.cBlueShift = 0;
		pfd.cAlphaBits = 0;															// no alpha buffer
		pfd.cAlphaShift = 0;														// shift bit ignored
		pfd.cAccumBits = 0;															// no accumulation buffer
		pfd.cAccumRedBits = 0;														// accum bits ignored
		pfd.cAccumGreenBits = 0;
		pfd.cAccumBlueBits = 0;
		pfd.cAccumAlphaBits = 0;
		pfd.cDepthBits = 32;														// 32-bit z-buffer
		pfd.cStencilBits = 0;														// no stencil buffer
		pfd.cAuxBuffers = 0;														// no auxiliary buffer
		pfd.iLayerType = PFD_MAIN_PLANE;											// main layer
		pfd.bReserved = 0;															// reserved
		pfd.dwLayerMask = 0;														// layer masks ignored
		pfd.dwVisibleMask = 0;
		pfd.dwDamageMask = 0;

		m_Internal->DC = GetDC(static_cast<HWND>(m_Window->Win32TryGetHWnd()));
		CT_CORE_ASSERT(m_Internal->DC, "Failed to get device context.");

		m_Internal->PF = ChoosePixelFormat(m_Internal->DC, &pfd);
		CT_CORE_ASSERT(m_Internal->PF != 0, "Failed to choose pixel format.");
		CT_CORE_ASSERT(SetPixelFormat(m_Internal->DC, m_Internal->PF, &pfd), "Failed to set pixel format.");

		m_Internal->RC = wglCreateContext(m_Internal->DC); // replace
		CT_CORE_ASSERT(m_Internal->RC, "Failed to create rendering context.");
		CT_CORE_ASSERT(wglMakeCurrent(m_Internal->DC, m_Internal->RC), "Failed to make rendering context current.");

		//CT_CORE_ASSERT(glewInit() == GLEW_OK, "GLEW Error!");

		// Extensions
		int attributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,		//m_MajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,		//m_MinorVersion,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,				//m_Profile, WGL_CONTEXT_CORE_PROFILE_BIT_ARB or WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
			//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		if (!wglewIsSupported("WGL_ARB_create_context"))
		{
			CT_CORE_ASSERT(false, "WGL_ARB_create_context not supported.");
		}

		m_Internal->RC = wglCreateContextAttribsARB(m_Internal->DC, 0, attributes);
		CT_CORE_ASSERT(m_Internal->RC != NULL, "Failed to create modern rendering context.");
		CT_CORE_ASSERT(wglMakeCurrent(m_Internal->DC, m_Internal->RC), "Failed to make modern rendering context current.");
	}
	void ContextGL::SwapBuffers()
	{
		::SwapBuffers(m_Internal->DC);
	}

	void ContextGL::SetVSync(const int vSync)
	{
		if (wglSwapIntervalEXT != NULL)
		{
			wglSwapIntervalEXT(vSync); // 0 - off, 1 - on, 2 - half
		}
	}
}
#endif
#endif