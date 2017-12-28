#include <fcntl.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "glxc.h"

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

bool glContext::s_ContextErrorOccurred = false;
int glContext::ContextErrorHandler(Display *dpy, XErrorEvent *ev)
{
    s_ContextErrorOccurred = true;
    return 0;
}

glContext::glContext()
 : m_Context(0)
{
	m_Display = XOpenDisplay(NULL);
	if (nullptr == m_Display)
	{
		throw std::runtime_error("Can't get display.");
	}
	GLXFBConfig fbc;
	m_Window = Createwindow(m_Display, fbc);
	
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

	int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&glContext::ContextErrorHandler);

	if (nullptr == glXCreateContextAttribsARB)
	{
		m_Context = glXCreateNewContext(m_Display, fbc, GLX_RGBA_TYPE, 0, True);
	}
	else
	{
		int context_attribs[] =
		{
        	GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        	GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        	None,                          None,
		};

    	m_Context = glXCreateContextAttribsARB(m_Display, fbc, 0, True, context_attribs);

		XSync(m_Display, False);
		if(s_ContextErrorOccurred || 0 == m_Context)
		{
			context_attribs[1] = 1;
			context_attribs[3] = 0;
			
			s_ContextErrorOccurred = false;
		
			m_Context = glXCreateContextAttribsARB(m_Display, fbc, 0, True, context_attribs);
		}
	}

	XSync(m_Display, False);
	XSetErrorHandler(oldHandler);

	if (s_ContextErrorOccurred || 0 == m_Context)
	{
		throw std::runtime_error("Failed to create glx context.");
	}
	glXMakeCurrent(m_Display, m_Window, m_Context);

	glXQueryDrawable(m_Display, m_Window, GLX_WIDTH, &m_Width);
	glXQueryDrawable(m_Display, m_Window, GLX_HEIGHT, &m_Height);
	
	glewExperimental = true;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		throw std::runtime_error("GLEW initialize failed.");
	}
	
#if defined(_DEBUG)
    std::cout << "Vendor:    " << (const char*)glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer:  " << (const char*)glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version:   " << (const char*)glGetString(GL_VERSION) << std::endl;
#endif
}

glContext::~glContext()
{
	glXMakeCurrent(m_Display, 0, 0);
	glXDestroyContext(m_Display, m_Context);
	Destroywindow(m_Window, m_Colormap, m_Display);
	XCloseDisplay(m_Display);
}

void glContext::SwapBuffers() const
{
	glXSwapBuffers(m_Display, m_Window);
}


Window glContext::Createwindow(Display* display, GLXFBConfig& config)
{
	int visual_attribs[] =
	{
		GLX_X_RENDERABLE,  True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,   GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE,      8,
		GLX_GREEN_SIZE,    8,
		GLX_BLUE_SIZE,     8,
		GLX_ALPHA_SIZE,    8,
		GLX_DEPTH_SIZE,    24,
		GLX_STENCIL_SIZE,  8,
		GLX_DOUBLEBUFFER,  True,
		None,              None,
	};

	int glx_major, glx_minor;
 
	if( !glXQueryVersion( display, &glx_major, &glx_minor )
		|| ((glx_major == 1) && (glx_minor < 3 ))
		|| ( glx_major < 1 ))
	{
		throw std::runtime_error("Can't query glx version.");
	}

	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	if (!fbc)
	{
		throw std::runtime_error("Can't choose fbconfig.");
	}
	
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
	for (int i = 0; i < fbcount; ++i)
	{
    	XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
		if (nullptr != vi)
		{
      		int samp_buf, samples;
      		glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
      		glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );
      
			if ((best_fbc < 0) || (samp_buf && samples > best_num_samp))
			{
				best_fbc = i;
				best_num_samp = samples;
			}
			if ((worst_fbc < 0) || (!samp_buf || samples < worst_num_samp))
			{
				worst_fbc = i;
				worst_num_samp = samples;
			}
		    XFree(vi);
		}
	}

	config = fbc[best_fbc];
	XFree(fbc);

	XVisualInfo *vi = glXGetVisualFromFBConfig(display, config);
	std::cout << "Chosen visual ID = 0x" << std::hex << vi->visualid << std::dec << std::endl;

	XSetWindowAttributes swa;

  	swa.colormap = m_Colormap = XCreateColormap(display, RootWindow( display, vi->screen ), vi->visual, AllocNone);
	swa.background_pixmap = None ;
	swa.border_pixel      = 0;
	swa.event_mask        = StructureNotifyMask;

	Window win = XCreateWindow(display,
	                          RootWindow(display, vi->screen), 
                              0, 360, 960, 360, 0, vi->depth, InputOutput, 
                              vi->visual, 
                              CWBorderPixel | CWColormap | CWEventMask,
                              &swa);
	if (!win)
	{
		throw std::runtime_error("Can't create window.");
    }

	XFree(vi);
	XStoreName(display, win, "GL Window");

	XMapWindow(display, win);
	
	return win;
}


void glContext::Destroywindow(Window window, Colormap colormap, Display* display)
{
	XDestroyWindow(display, window);
	XFreeColormap(display, colormap);
}
