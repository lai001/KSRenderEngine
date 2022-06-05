#include "D3D11/D3D11RasterizerState.hpp"
#include <assert.h>
#include <unordered_map>

namespace ks
{
	D3D11RasterizerState::D3D11RasterizerState(const RasterizerStateDescription& rasterizerStateDescription,
		const D3D11RenderEngineInfo& engineInfo)
		:engineInfo(engineInfo), rasterizerStateDescription(rasterizerStateDescription)
	{
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;

		std::unordered_map<RasterizerStateFillMode, D3D11_FILL_MODE> fillModeDic;
		fillModeDic[RasterizerStateFillMode::solid] = D3D11_FILL_SOLID;
		fillModeDic[RasterizerStateFillMode::wireframe] = D3D11_FILL_WIREFRAME;

		std::unordered_map<RasterizerStateCullMode, D3D11_CULL_MODE> cullModeDic;
		cullModeDic[RasterizerStateCullMode::back] = D3D11_CULL_BACK;
		cullModeDic[RasterizerStateCullMode::front] = D3D11_CULL_FRONT;
		cullModeDic[RasterizerStateCullMode::none] = D3D11_CULL_NONE;

		HRESULT status = S_OK;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = fillModeDic.at(rasterizerStateDescription.fillMode);
		desc.CullMode = cullModeDic.at(rasterizerStateDescription.cullMode);
		desc.FrontCounterClockwise = rasterizerStateDescription.frontCounterClockwise;
		desc.DepthBias = rasterizerStateDescription.depthBias;
		desc.DepthBiasClamp = rasterizerStateDescription.depthBiasClamp;
		desc.SlopeScaledDepthBias = rasterizerStateDescription.slopeScaledDepthBias;
		desc.DepthClipEnable = rasterizerStateDescription.depthClipEnable;
		desc.ScissorEnable = rasterizerStateDescription.scissorEnable;
		desc.MultisampleEnable = rasterizerStateDescription.multisampleEnable;
		desc.AntialiasedLineEnable = rasterizerStateDescription.antialiasedLineEnable;
		status = d3dDevice->CreateRasterizerState(&desc, &rasterizerState);
		assert(status == S_OK);
	}

	D3D11RasterizerState::~D3D11RasterizerState()
	{
		assert(rasterizerState);
		rasterizerState->Release();
	}

	void D3D11RasterizerState::bind() const
	{
		assert(rasterizerState);
		assert(engineInfo.getContext());
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.getContext();
		d3dDeviceContext->RSSetState(rasterizerState);
	}

	void D3D11RasterizerState::unbind() const
	{
	}
}