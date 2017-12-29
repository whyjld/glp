#ifndef _FRAME_BUFFER_h_
#define _FRAME_BUFFER_h_

#include "globj.h"
#include "texture.h"
#include "renderbuffer.h"

class Framebuffer : public GLObj<GLuint, Framebuffer>
{
public:
	Framebuffer();
	Framebuffer(const Framebuffer& v);	
	Framebuffer(Framebuffer&& v);

	~Framebuffer();

	Framebuffer& operator=(const Framebuffer& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Framebuffer>::operator=(v);
	    }
	    return *this;
	}
	
	Framebuffer& operator=(Framebuffer&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Framebuffer>::operator=(v);
	    }
	    return *this;
	}
	
	void Bind(GLenum target);
	void Unbind(GLenum target);
	
	void Attache(GLenum target, GLenum attachment, Texture&& tex);
	void Attache(GLenum target, GLenum attachment, Renderbuffer&& buf);
	
	bool CheckStatus();

    static void DeleteObject(GLuint obj);
protected:
    GLuint CreateObject();
private:
};

#endif//_FRAME_BUFFER_h_