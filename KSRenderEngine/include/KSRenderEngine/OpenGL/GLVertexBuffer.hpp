#ifndef KSRenderEngine_OpenGL_GLVertexBuffer_hpp
#define KSRenderEngine_OpenGL_GLVertexBuffer_hpp

#include <Foundation/Foundation.hpp>
#include "Interface/VertexBuffer.hpp"
#include "Interface/VertexBufferLayout.hpp"

namespace ks
{
	class GLVertexBuffer : public noncopyable, public IVertexBuffer
	{
	private:
		unsigned int RendererID;
		const VertexBufferLayout layout;

	public:
		GLVertexBuffer(const void * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const VertexBufferLayout& layout);
		~GLVertexBuffer();

		void bind() const override;
		void unbind() const override;

		VertexBufferLayout getLayout() const;

		void applyLayout() const;

		unsigned int getRendererID() const;
	};
}

#endif // !KSRenderEngine_OpenGL_GLVertexBuffer_hpp
