#include "D3D11/D3D11RenderBuffer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace ks
{
	D3D11RenderBuffer::D3D11RenderBuffer(void const * vertexBuffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const IShader & shader,
		const void * indexBufferData,
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type,
		const D3D11RenderEngineInfo& engineInfo)
		:engineInfo(engineInfo)
	{
		this->shader = dynamic_cast<const D3D11Shader*>(&shader);
		assert(this->shader->getVertexBufferLayout().getStride() == vertexStride);
		vertextBuffer = std::make_unique<const D3D11VertexBuffer>(vertexBuffer, vertexCount, vertexStride, engineInfo);
		indexBuffer = std::make_unique<const D3D11IndexBuffer>(indexBufferData, indexCount, type, engineInfo);
	}

	D3D11RenderBuffer::~D3D11RenderBuffer()
	{

	}

	void D3D11RenderBuffer::commit(const IFrameBuffer * frameBuffer)
	{
		const D3D11FrameBuffer* d3d11FrameBuffer = nullptr;
		if (frameBuffer)
		{
			d3d11FrameBuffer = dynamic_cast<const D3D11FrameBuffer*>(frameBuffer);
		}
		assert(vertextBuffer);
		assert(indexBuffer);
		assert(engineInfo.getContext());
		assert(engineInfo.device);
		assert(shader);
		assert(blendState);
		assert(depthStencilState);
		assert(rasterizerState);
		ID3D11DeviceContext& d3dDeviceContext = *engineInfo.getContext();

		if (d3d11FrameBuffer)
		{
			d3d11FrameBuffer->bind();
		}
		vertextBuffer->bind();
		indexBuffer->bind();
		shader->bind();
		blendState->bind();
		depthStencilState->bind();
		rasterizerState->bind();
		if (clearBufferFlags.isContains(ClearBufferFlags::color) && d3d11FrameBuffer)
		{
			d3dDeviceContext.ClearRenderTargetView(d3d11FrameBuffer->getNativeRenderTargetView(), glm::value_ptr(clearColor));
		}
		d3dDeviceContext.RSSetViewports(1, &viewport);
		d3dDeviceContext.RSSetScissorRects(1, &scissorRect);
		d3dDeviceContext.IASetPrimitiveTopology(getPrimitiveTopology(primitiveTopologyType));
		d3dDeviceContext.DrawIndexed(indexBuffer->getCount(), 0, 0);

		const D3D11_DEVICE_CONTEXT_TYPE contextType = d3dDeviceContext.GetType();
		if (contextType == D3D11_DEVICE_CONTEXT_DEFERRED)
		{
			ID3D11CommandList* d3dCommandList = nullptr;
			HRESULT status = d3dDeviceContext.FinishCommandList(FALSE, &d3dCommandList);
			assert(SUCCEEDED(status));
			D3D11RenderEngine* renderEngine = dynamic_cast<D3D11RenderEngine*>(engineInfo.renderEngine);
			assert(renderEngine);
			renderEngine->addRenderCommand([this, d3dCommandList]()
			{
				engineInfo.immediateContext->ExecuteCommandList(d3dCommandList, TRUE);
				d3dCommandList->Release();
			});
		}
		if (d3d11FrameBuffer)
		{
			d3d11FrameBuffer->unbind();
		}
	}

	D3D11_PRIMITIVE_TOPOLOGY D3D11RenderBuffer::getPrimitiveTopology(const ks::PrimitiveTopologyType &primitiveTopologyType) const
	{
		std::unordered_map<PrimitiveTopologyType, D3D11_PRIMITIVE_TOPOLOGY> dic;
		dic[PrimitiveTopologyType::trianglelist] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		dic[PrimitiveTopologyType::linelist] = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		dic[PrimitiveTopologyType::pointlist] = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		dic[PrimitiveTopologyType::trianglestrip] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		dic[PrimitiveTopologyType::linestrip] = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		assert(dic.end() != dic.find(primitiveTopologyType));
		return dic.at(primitiveTopologyType);
	}

	void D3D11RenderBuffer::setClearBufferFlags(const ks::ClearBufferFlags & flags)
	{
		clearBufferFlags = flags;
	}

	void D3D11RenderBuffer::setClearColor(const glm::vec4 & clearColor)
	{
		this->clearColor = clearColor;
	}

	void D3D11RenderBuffer::setViewport(const int x, const int y, const int width, const int height)
	{
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		scissorRect.left = x;
		scissorRect.top = y;
		scissorRect.right = x + width;
		scissorRect.bottom = y + height;
	}

	void D3D11RenderBuffer::setBlendState(const IBlendState & blendState)
	{
		this->blendState = &blendState;
	}

	void D3D11RenderBuffer::setDepthStencilState(const IDepthStencilState & depthStencilState)
	{
		this->depthStencilState = &depthStencilState;
	}

	void D3D11RenderBuffer::setRasterizerState(const IRasterizerState & rasterizerState)
	{
		this->rasterizerState = &rasterizerState;
	}

	void D3D11RenderBuffer::setPrimitiveTopologyType(const ks::PrimitiveTopologyType &primitiveTopologyType)
	{
		assert(primitiveTopologyType == PrimitiveTopologyType::trianglelist);
		this->primitiveTopologyType = primitiveTopologyType;
	}
}