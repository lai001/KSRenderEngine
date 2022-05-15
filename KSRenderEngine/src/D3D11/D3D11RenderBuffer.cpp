#include "D3D11/D3D11RenderBuffer.hpp"

namespace ks
{
	D3D11RenderBuffer::D3D11RenderBuffer(void const * vertexBuffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const void * indexBufferData,
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type,
		const D3D11RenderEngineInfo& engineInfo)
		:engineInfo(engineInfo)
	{
		vertextBuffer = std::make_unique<const D3D11VertexBuffer>(vertexBuffer, vertexCount, vertexStride, engineInfo);
		indexBuffer = std::make_unique<const D3D11IndexBuffer>(indexBufferData, indexCount, type, engineInfo);
	}

	D3D11RenderBuffer::~D3D11RenderBuffer()
	{

	}

	void D3D11RenderBuffer::commit()
	{
		assert(vertextBuffer);
		assert(indexBuffer);
		assert(engineInfo.context);
		assert(engineInfo.device);
		assert(shader);
		assert(blendState);
		assert(depthStencilState);
		assert(rasterizerState);
		assert(frameBuffer);
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;

		shader->bind();
		vertextBuffer->bind();
		indexBuffer->bind();
		blendState->bind();
		depthStencilState->bind();
		rasterizerState->bind();
		frameBuffer->bind();
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dDeviceContext->DrawIndexed(indexBuffer->getCount(), 0, 0);
	}

	void D3D11RenderBuffer::setShader(const IShader & shader)
	{
		this->shader = &shader;
	}

	void D3D11RenderBuffer::setFrameBuffer(const IFrameBuffer & frameBuffer)
	{
		this->frameBuffer = &frameBuffer;
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
		this->primitiveTopologyType = primitiveTopologyType;
	}
}