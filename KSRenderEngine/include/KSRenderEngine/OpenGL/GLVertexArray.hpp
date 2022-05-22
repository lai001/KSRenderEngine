#ifndef KSRenderEngine_OpenGL_GLVertexArray_hpp
#define KSRenderEngine_OpenGL_GLVertexArray_hpp

#include <Foundation/Foundation.hpp>

namespace ks
{
	class GLVertexArray : public noncopyable
	{
	private:
		unsigned int RendererID;
	public:
		GLVertexArray();
		~GLVertexArray();

		int getRendererID();

		void bind() const;
		void unbind();
	};
}

#endif // ! KSRenderEngine_OpenGL_GLVertexArray_hpp