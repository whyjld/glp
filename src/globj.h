#ifndef _GL_OBJ_h_
#define _GL_OBJ_h_

#include "glcontext.h"
#include <algorithm>

template<typename _T, typename _D>
class GLObj
{
public:
	GLObj(_T o)
	 : m_Obj(o)
	 , m_Usage(new size_t(1))
	{
	}
	
	GLObj(const GLObj<_T, _D>& v)
	 : m_Obj(v.m_Obj)
	 , m_Usage(v.m_Usage)
	{
	    if(nullptr != m_Usage)
	    {
	        ++(*m_Usage);
	    }
	}
	
	GLObj(GLObj<_T, _D>&& v)
	 : m_Obj(v.m_Obj)
	 , m_Usage(v.m_Usage)
	{
	    v.m_Usage = nullptr;
	}
	~GLObj()
	{
	    Release(m_Obj, m_Usage);
	}
	
	GLObj<_T, _D>& operator=(const GLObj<_T, _D>& v)
	{
	    if(this != &v)
	    {
	        _T obj = m_Obj;
	        size_t* usage = m_Usage;
	        
	        m_Obj = v.m_Obj;
	        m_Usage = v.m_Usage;

    	    if(nullptr != m_Usage)
	        {
	            ++(*m_Usage);
	        }
	        
	        Release(obj, usage);
	    }
	    return *this;
	}
	
	GLObj<_T, _D>& operator=(GLObj<_T, _D>&& v)
	{
	    if(this != &v)
	    {
	        std::swap(m_Obj, v.m_Obj);
	        std::swap(m_Usage, v.m_Usage);
	    }
	    return *this;
	}
	
	operator _T() const
	{
	    return m_Obj;
	}
	
	_T getGLObj() const
	{
	    return m_Obj;
	}
protected:
    void Release(_T obj, size_t* usage)
    {
	    if(nullptr != usage && 0 == --(*usage))
	    {
	        if(0 != obj)
	        {
	            _D::DeleteObject(obj);
	        }
	        delete usage;
	    }
    }

	_T m_Obj;
	size_t* m_Usage;
};

#endif//_GL_OBJ_h_