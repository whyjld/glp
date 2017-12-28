#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string.h>
#include <stdexcept>

#ifdef EGL_USE_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#include "eglc.h"

PFNGLTEXDIRECTVIV glTexDirectVIV = NULL;
PFNGLTEXDIRECTINVALIDATEVIV glTexDirectInvalidateVIV = NULL;

glContext::glContext()
{
	m_NativeDisplay = GetNativeDisplay();
	m_Display = eglGetDisplay(m_NativeDisplay);
	if (nullptr == m_Display)
	{
		throw std::runtime_error("Can't get display.");
	}
	eglInitialize(m_Display, nullptr, nullptr);
	eglBindAPI(EGL_OPENGL_ES_API);

	static const EGLint config[] =
	{
		EGL_RED_SIZE,	5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE,	5,
		EGL_ALPHA_SIZE, 0,
		EGL_SAMPLES,	0,
		EGL_NONE,       EGL_NONE,
	};

	EGLint numconfigs;
	EGLConfig eglconfig;
	eglChooseConfig(m_Display, config, &eglconfig, 1, &numconfigs);
	if(0 == numconfigs)
	{
		throw std::runtime_error("Can't choose config.");
	}

	m_NativeWindow = Createwindow(m_Display, m_NativeDisplay);	
	if ((EGLNativeWindowType)0 == m_NativeWindow)
	{
		throw std::runtime_error("Can't create native window.");
	}

	m_Surface = eglCreateWindowSurface(m_Display, eglconfig, m_NativeWindow, nullptr);
	if (nullptr == m_Surface)
	{
		throw std::runtime_error("Can't create surface.");
	}

	EGLint ContextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	m_Context = eglCreateContext( m_Display, eglconfig, EGL_NO_CONTEXT, ContextAttribList );
	eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);
	
	if (nullptr == glTexDirectVIV)
	{
		glTexDirectVIV = (PFNGLTEXDIRECTVIV)eglGetProcAddress("glTexDirectVIV");
	}

	if (nullptr == glTexDirectInvalidateVIV)
	{
		glTexDirectInvalidateVIV = (PFNGLTEXDIRECTINVALIDATEVIV)eglGetProcAddress("glTexDirectInvalidateVIV");
	}

	EGLint width = 0;
	EGLint height = 0;
	eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &width);
	eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &height);
	
	m_Width = width;
	m_Height = height;
}

glContext::~glContext()
{
	eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_Display, m_Context);
	eglDestroySurface(m_Display, m_Surface);	
	Destroywindow(m_NativeWindow, m_NativeDisplay);
	eglTerminate(m_Display);
	eglReleaseThread();	
}

void glContext::SwapBuffers() const
{
	eglSwapBuffers(m_Display, m_Surface);
}

EGLNativeDisplayType glContext::GetNativeDisplay()
{
	EGLNativeDisplayType eglNativeDisplayType = NULL;
#if (defined EGL_USE_X11)
	eglNativeDisplayType = XOpenDisplay(NULL);
#elif (defined EGL_API_FB)
	eglNativeDisplayType = fbGetDisplayByIndex(0); //Pass the argument as required to show the framebuffer	
#else	
	display = EGL_DEFAULT_DISPLAY;
#endif	
	return eglNativeDisplayType;
}

EGLNativeWindowType glContext::Createwindow(EGLDisplay egldisplay, EGLNativeDisplayType eglNativeDisplayType)
{
	EGLNativeWindowType native_window = (EGLNativeWindowType)0;

#if (defined EGL_USE_X11)
	Window window, rootwindow;
	int screen = DefaultScreen(eglNativeDisplayType);		
	rootwindow = RootWindow(eglNativeDisplayType,screen); 
	window = XCreateSimpleWindow(eglNativeDisplayType, rootwindow, 0, 0, 400, 533, 0, 0, WhitePixel (eglNativeDisplayType, screen));
	XMapWindow(eglNativeDisplayType, window);
	native_window = window;	
#else
	const char *vendor = eglQueryString(egldisplay, EGL_VENDOR);
	if (strstr(vendor, "Imagination Technologies"))
		native_window = (EGLNativeWindowType)0;
	else if (strstr(vendor, "AMD"))
		native_window = (EGLNativeWindowType)  open("/dev/fb0", O_RDWR);
	else if (strstr(vendor, "Vivante")) //NEEDS FIX - functs don't exist on other platforms
	{
#if (defined EGL_API_FB)
		native_window = fbCreateWindow(eglNativeDisplayType, 0, 0, 0, 0);
#endif		
	}
	else
	{
		printf("Unknown vendor [%s]\n", vendor);
		return 0;
	}	
#endif
	return native_window;

}


void glContext::Destroywindow(EGLNativeWindowType eglNativeWindowType, EGLNativeDisplayType eglNativeDisplayType)
{
	(void) eglNativeWindowType;
#if (defined EGL_USE_X11)
	//close x display		
	XCloseDisplay(eglNativeDisplayType);
#endif
}
