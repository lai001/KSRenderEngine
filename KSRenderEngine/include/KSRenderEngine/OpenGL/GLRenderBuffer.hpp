#ifndef KSRenderEngineGLRenderBuffer
#define KSRenderEngineGLRenderBuffer

#include "Interface/RenderBuffer.hpp"
#include "GLVertexArray.hpp"
#include "GLIndexBuffer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLShader.hpp"

namespace ks
{
	class GLRenderBuffer : public noncopyable, public IRenderBuffer
	{
	private:
		const GLShader* shader = nullptr;
		std::unique_ptr<GLVertexArray> vertexArray = nullptr;
		std::unique_ptr<GLVertexBuffer> vertextBuffer = nullptr;
		std::unique_ptr<GLIndexBuffer> indexBuffer = nullptr;
		const IFrameBuffer* frameBuffer = nullptr;

	public:
		GLRenderBuffer(const void * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const VertexBufferLayout & layout,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type);

		void commit() override;
		void setShader(const IShader & shader) override;
		void setFrameBuffer(const IFrameBuffer & frameBuffer) override;
		void setBlendState(const IBlendState & blendState) override;
		void setDepthStencilState(const IDepthStencilState & depthStencilState) override;
		void setRasterizerState(const IRasterizerState & rasterizerState) override;
		void setPrimitiveTopologyType(const PrimitiveTopologyType & primitiveTopologyType) override;
	};

}

#endif // !KSRenderEngineGLRenderBuffer
