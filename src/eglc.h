#ifndef _EGLC_H_
#define _EGLC_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>

/* GL_VIV_direct_texture */
#ifndef GL_VIV_direct_texture
#define GL_VIV_YV12                     0x8FC0
#define GL_VIV_NV12                     0x8FC1
#define GL_VIV_YUY2                     0x8FC2
#define GL_VIV_UYVY                     0x8FC3
#define GL_VIV_NV21                     0x8FC4
#endif

typedef void (GL_APIENTRY *PFNGLTEXDIRECTVIV) (GLenum Target, GLsizei Width, GLsizei Height, GLenum Format, GLvoid ** Pixels);
typedef void (GL_APIENTRY *PFNGLTEXDIRECTINVALIDATEVIV) (GLenum Target);

extern PFNGLTEXDIRECTVIV glTexDirectVIV;
extern PFNGLTEXDIRECTINVALIDATEVIV glTexDirectInvalidateVIV;

class glContext
{
public:
	glContext();
	glContext(glContext&) = delete;
	
	~glContext();
	
	void SwapBuffers() const;
	
	GLuint getWidth() const
	{
		return m_Width;
	}
	
	GLuint getHeight() const
	{
		return m_Height;
	}	
private:
	EGLNativeDisplayType GetNativeDisplay();
	EGLNativeWindowType Createwindow(EGLDisplay egldisplay, EGLNativeDisplayType eglNativeDisplayType);
	void Destroywindow(EGLNativeWindowType eglNativeWindowType,EGLNativeDisplayType eglNativeDisplayType);

	EGLNativeDisplayType m_NativeDisplay;
	EGLNativeWindowType  m_NativeWindow;
	EGLDisplay			 m_Display;
	EGLSurface			 m_Surface;
	EGLContext			 m_Context;
	
	GLuint m_Width;
	GLuint m_Height;
};

#endif//_EGLC_H_

