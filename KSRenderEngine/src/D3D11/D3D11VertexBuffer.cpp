#include "D3D11/D3D11VertexBuffer.hpp"
#include <assert.h>

namespace ks
{
	constexpr unsigned int D3D11VertexBuffer::DefaultStartSlot = 0;


	D3D11VertexBuffer::D3D11VertexBuffer(void const * buffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const D3D11RenderEngineInfo & engineInfo)
		:engineInfo(engineInfo), vertexCount(vertexCount), vertexStride(vertexStride)
	{
		assert(buffer);
		assert(engineInfo.context);
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;

		HRESULT status = S_OK;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = vertexCount * vertexStride;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		status = d3dDevice->CreateBuffer(&desc, nullptr, &verteBuffer);
		assert(status == S_OK);
		D3D11_MAPPED_SUBRESOURCE vtx_resource;
		status = d3dDeviceContext->Map(verteBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource);
		assert(status == S_OK);
		memcpy(vtx_resource.pData, buffer, desc.ByteWidth);
		d3dDeviceContext->Unmap(verteBuffer, 0);
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		assert(verteBuffer);
		verteBuffer->Release();
	}

	void D3D11VertexBuffer::bind() const
	{
		assert(engineInfo.device);
		assert(verteBuffer);
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		const unsigned int offset = 0;
		d3dDeviceContext->IASetVertexBuffers(D3D11VertexBuffer::DefaultStartSlot, 1, &verteBuffer, &vertexStride, &offset);
	}

	void D3D11VertexBuffer::unbind() const
	{
		assert(engineInfo.device);
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		const unsigned int offset = 0;
		d3dDeviceContext->IASetVertexBuffers(D3D11VertexBuffer::DefaultStartSlot, 0, nullptr, nullptr, &offset);
	}
}