#ifndef KSRenderEngineRenderBuffer
#define KSRenderEngineRenderBuffer

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "FrameBuffer.hpp"

namespace ks
{
	class IRenderBuffer
	{
	public:
		virtual ~IRenderBuffer() = 0 { };
		virtual void commit() = 0;
		//virtual void setVertexBuffer(const IVertexBuffer& vertexBuffer) = 0;
		//virtual void setIndexBuffer(const IIndexBuffer& indexBuffer) = 0;
		virtual void setShader(const IShader& shader) = 0;
		virtual void setFrameBuffer(const IFrameBuffer& frameBuffer) = 0;
	};
}

#endif // !KSRenderEngineRenderBuffer