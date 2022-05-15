#include "D3D11\D3D11DepthStencilState.hpp"

namespace ks
{
	D3D11DepthStencilState::D3D11DepthStencilState(const DepthStencilStateDescription & depthStencilStateDescription,
		const D3D11RenderEngineInfo & engineInfo)
		:engineInfo(engineInfo), depthStencilStateDescription(depthStencilStateDescription)
	{
		assert(engineInfo.context);
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;
		// TODO:
		HRESULT status = S_OK;
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = depthStencilStateDescription.isDepthEnable;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = depthStencilStateDescription.isStencilEnable;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		status = d3dDevice->CreateDepthStencilState(&desc, &depthStencilState);
		assert(status == S_OK);
	}

	D3D11DepthStencilState::~D3D11DepthStencilState()
	{
		assert(depthStencilState);
		depthStencilState->Release();
	}

	void D3D11DepthStencilState::bind() const
	{
		assert(depthStencilState);
		assert(engineInfo.context);
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		d3dDeviceContext->OMSetDepthStencilState(depthStencilState, 0);
	}

	void D3D11DepthStencilState::unbind() const
	{
	}
}
