#include "D3D11/D3D11IndexBuffer.hpp"
#include <assert.h>

namespace ks
{
	D3D11IndexBuffer::D3D11IndexBuffer(const void* data,
		unsigned int count,
		const IndexDataType indexDataType,
		const D3D11RenderEngineInfo& engineInfo)
		: count(count), indexDataType(indexDataType), engineInfo(engineInfo)
	{
		assert(data);
		assert(engineInfo.context);
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;

		HRESULT status = S_OK;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = count * IIndexBuffer::getSizeOf(indexDataType);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		status = d3dDevice->CreateBuffer(&desc, nullptr, &indexBuffer);
		assert(status == S_OK);
		D3D11_MAPPED_SUBRESOURCE idx_resource;
		status = d3dDeviceContext->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource);
		assert(status == S_OK);
		memcpy(idx_resource.pData, data, desc.ByteWidth);
		d3dDeviceContext->Unmap(indexBuffer, 0);
	}

	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		assert(indexBuffer);
		indexBuffer->Release();
	}

	unsigned int D3D11IndexBuffer::getCount() const
	{
		return count;
	}

	void D3D11IndexBuffer::bind() const
	{
		assert(engineInfo.context);
		std::unordered_map<IIndexBuffer::IndexDataType, DXGI_FORMAT> indexDataTypeDic;
		indexDataTypeDic[IIndexBuffer::IndexDataType::uint16] = DXGI_FORMAT_R16_UINT;
		indexDataTypeDic[IIndexBuffer::IndexDataType::uint32] = DXGI_FORMAT_R32_UINT;
		assert(indexDataTypeDic.end() != indexDataTypeDic.find(indexDataType));
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		d3dDeviceContext->IASetIndexBuffer(indexBuffer, indexDataTypeDic.at(indexDataType), 0);
	}

	void D3D11IndexBuffer::unbind() const
	{
		assert(engineInfo.context);
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		d3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);
	}

	IIndexBuffer::IndexDataType D3D11IndexBuffer::getIndexDataType() noexcept
	{
		return indexDataType;
	}
}