#ifndef KSRenderEngine_D3D11_D3D11RenderBuffer_hpp
#define KSRenderEngine_D3D11_D3D11RenderBuffer_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <Foundation/Foundation.hpp>
#include <glm/glm.hpp>
#include "Common/Common.hpp"
#include "Interface/RenderBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11VertexBuffer.hpp"
#include "D3D11/D3D11IndexBuffer.hpp"
#include "D3D11/D3D11FrameBuffer.hpp"
#include "D3D11/D3D11Shader.hpp"

namespace ks
{
	class D3D11RenderBuffer : public boost::noncopyable, public IRenderBuffer
	{
	public:
		D3D11RenderBuffer(void const * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const IShader & shader,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11RenderBuffer();

		void setClearBufferFlags(const ks::ClearBufferFlags & flags) override;
		void setClearColor(const glm::vec4 & clearColor) override;
		void setViewport(const int x, const int y, const int width, const int height) override;
		void setBlendState(const IBlendState & blendState) override;
		void setDepthStencilState(const IDepthStencilState & depthStencilState) override;
		void setRasterizerState(const IRasterizerState & rasterizerState) override;
		void setPrimitiveTopologyType(const PrimitiveTopologyType &primitiveTopologyType);
		void commit(const IFrameBuffer & frameBuffer) override;

	private:
		const IBlendState* blendState = nullptr;
		const IDepthStencilState* depthStencilState = nullptr;
		const IRasterizerState* rasterizerState = nullptr;
		const D3D11Shader* shader = nullptr;

		std::unique_ptr<const D3D11VertexBuffer> vertextBuffer = nullptr;
		std::unique_ptr<const D3D11IndexBuffer> indexBuffer = nullptr;

		PrimitiveTopologyType primitiveTopologyType;
		D3D11_VIEWPORT viewport;
		D3D11_RECT scissorRect;

		glm::vec4 clearColor;
		D3D11RenderEngineInfo engineInfo;

		ClearBufferFlags clearBufferFlags;

		D3D11_PRIMITIVE_TOPOLOGY getPrimitiveTopology(const ks::PrimitiveTopologyType &primitiveTopologyType) const;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RenderBuffer_hpp