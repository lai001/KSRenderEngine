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

		glm::vec4 _clearColor;
		glm::ivec4 _viewport;
		ks::ClearBufferFlags clearBufferFlags;
		void clear() const;
		void clearColor() const;
		void viewport() const;

	public:
		GLRenderBuffer(const void * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const IShader & shader,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type);

		void commit(const IFrameBuffer& frameBuffer) override;
		void setClearBufferFlags(const ks::ClearBufferFlags& flags) override;
		void setClearColor(const glm::vec4& clearColor) override;
		void setViewport(const int x, const int y, const int width, const int height) override;
		void setBlendState(const IBlendState & blendState) override;
		void setDepthStencilState(const IDepthStencilState & depthStencilState) override;
		void setRasterizerState(const IRasterizerState & rasterizerState) override;
		void setPrimitiveTopologyType(const PrimitiveTopologyType & primitiveTopologyType) override;
	};

}

#endif // !KSRenderEngineGLRenderBuffer
