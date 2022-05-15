#ifndef KSRenderEngine_D3D11_D3D11RenderBuffer_hpp
#define KSRenderEngine_D3D11_D3D11RenderBuffer_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <Foundation/Foundation.hpp>
#include "Common/Common.hpp"
#include "Interface/RenderBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11VertexBuffer.hpp"
#include "D3D11/D3D11IndexBuffer.hpp"

namespace ks
{
	class D3D11RenderBuffer : public boost::noncopyable, public IRenderBuffer
	{
	public:
		D3D11RenderBuffer(void const * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11RenderBuffer();

		void commit() override;
		void setShader(const IShader & shader) override;
		void setFrameBuffer(const IFrameBuffer & frameBuffer) override;
		void setBlendState(const IBlendState & blendState) override;
		void setDepthStencilState(const IDepthStencilState & depthStencilState) override;
		void setRasterizerState(const IRasterizerState & rasterizerState) override;
		void setPrimitiveTopologyType(const PrimitiveTopologyType &primitiveTopologyType);

	private:
		const IBlendState* blendState = nullptr;
		const IDepthStencilState* depthStencilState = nullptr;
		const IRasterizerState* rasterizerState = nullptr;
		const IShader* shader = nullptr;
		const IFrameBuffer* frameBuffer = nullptr;

		std::unique_ptr<const D3D11VertexBuffer> vertextBuffer = nullptr;
		std::unique_ptr<const D3D11IndexBuffer> indexBuffer = nullptr;

		//std::vector<D3D11_VIEWPORT> viewports;
		//std::vector<D3D11_RECT> scissorRects;
		PrimitiveTopologyType primitiveTopologyType;

		D3D11RenderEngineInfo engineInfo;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RenderBuffer_hpp