#include <D3D11/D3D11BlendState.hpp>
#include <unordered_map>
#include <assert.h>

namespace ks
{
	D3D11BlendState::D3D11BlendState(const BlendStateDescription::Addition& addition,
		const BlendStateDescription& description,
		const D3D11RenderEngineInfo& engineInfo)
		:engineInfo(engineInfo), blendStateDescription(description)
	{
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;
		HRESULT status = S_OK;
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = addition.isAlphaToCoverageEnable;
		desc.IndependentBlendEnable = addition.isIndependentBlendEnable;
		desc.RenderTarget[0] = toD3DBlendDesc(description);
		status = d3dDevice->CreateBlendState(&desc, &blendState);
		assert(status == S_OK);
	}

	D3D11BlendState::~D3D11BlendState()
	{
		assert(blendState);
		blendState->Release();
	}

	D3D11_RENDER_TARGET_BLEND_DESC D3D11BlendState::toD3DBlendDesc(const BlendStateDescription & description)
	{
		D3D11_COLOR_WRITE_ENABLE mask = static_cast<D3D11_COLOR_WRITE_ENABLE>(0);
		if (description.renderTargetWriteFlag.isContains(ColorWriteEnableFlag::all()))
		{
			mask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		else if (description.renderTargetWriteFlag.isContains(ColorWriteEnableFlag::red))
		{
			mask = static_cast<D3D11_COLOR_WRITE_ENABLE>(mask | D3D10_COLOR_WRITE_ENABLE_RED);
		}
		else if (description.renderTargetWriteFlag.isContains(ColorWriteEnableFlag::green))
		{
			mask = static_cast<D3D11_COLOR_WRITE_ENABLE>(mask | D3D10_COLOR_WRITE_ENABLE_GREEN);
		}
		else if (description.renderTargetWriteFlag.isContains(ColorWriteEnableFlag::blue))
		{
			mask = static_cast<D3D11_COLOR_WRITE_ENABLE>(mask | D3D10_COLOR_WRITE_ENABLE_BLUE);
		}
		else if (description.renderTargetWriteFlag.isContains(ColorWriteEnableFlag::alpha))
		{
			mask = static_cast<D3D11_COLOR_WRITE_ENABLE>(mask | D3D10_COLOR_WRITE_ENABLE_ALPHA);
		}
		else
		{
			assert(false);
		}

		std::unordered_map<BlendType, D3D11_BLEND> blendTypeDic;
		blendTypeDic[BlendType::zero] = D3D11_BLEND_ZERO;
		blendTypeDic[BlendType::one] = D3D11_BLEND_ONE;
		blendTypeDic[BlendType::srcColor] = D3D11_BLEND_SRC_COLOR;
		blendTypeDic[BlendType::invSrcColor] = D3D11_BLEND_INV_SRC_COLOR;
		blendTypeDic[BlendType::srcAlpha] = D3D11_BLEND_SRC_ALPHA;
		blendTypeDic[BlendType::invSrcAlpha] = D3D11_BLEND_INV_SRC_ALPHA;
		blendTypeDic[BlendType::destAlpha] = D3D11_BLEND_DEST_ALPHA;
		blendTypeDic[BlendType::invDestAlpha] = D3D11_BLEND_INV_DEST_ALPHA;
		blendTypeDic[BlendType::destColor] = D3D11_BLEND_DEST_COLOR;
		blendTypeDic[BlendType::invDestColor] = D3D11_BLEND_INV_DEST_COLOR;
		blendTypeDic[BlendType::srcAlphaSat] = D3D11_BLEND_SRC_ALPHA_SAT;
		blendTypeDic[BlendType::blendFactor] = D3D11_BLEND_BLEND_FACTOR;
		blendTypeDic[BlendType::invBlendFactor] = D3D11_BLEND_INV_BLEND_FACTOR;
		blendTypeDic[BlendType::src1Color] = D3D11_BLEND_SRC1_COLOR;
		blendTypeDic[BlendType::invSrc1Color] = D3D11_BLEND_INV_SRC1_COLOR;
		blendTypeDic[BlendType::src1Alpha] = D3D11_BLEND_SRC1_ALPHA;
		blendTypeDic[BlendType::invSrc1Alpha] = D3D11_BLEND_INV_SRC1_ALPHA;

		std::unordered_map<BlendOpType, D3D11_BLEND_OP> blendOpTypeDic;
		blendOpTypeDic[BlendOpType::add] = D3D11_BLEND_OP_ADD;
		blendOpTypeDic[BlendOpType::subtract] = D3D11_BLEND_OP_SUBTRACT;
		blendOpTypeDic[BlendOpType::revSubtract] = D3D11_BLEND_OP_REV_SUBTRACT;
		blendOpTypeDic[BlendOpType::min] = D3D11_BLEND_OP_MIN;
		blendOpTypeDic[BlendOpType::max] = D3D11_BLEND_OP_MAX;

		assert(blendOpTypeDic.end() != blendOpTypeDic.find(description.blendOp));
		assert(blendOpTypeDic.end() != blendOpTypeDic.find(description.blendOpAlpha));
		assert(blendTypeDic.end() != blendTypeDic.find(description.srcBlend));
		assert(blendTypeDic.end() != blendTypeDic.find(description.destBlend));
		assert(blendTypeDic.end() != blendTypeDic.find(description.srcBlendAlpha));
		assert(blendTypeDic.end() != blendTypeDic.find(description.destBlendAlpha));

		D3D11_RENDER_TARGET_BLEND_DESC d3dBlendDesc;
		d3dBlendDesc.BlendEnable = description.isBlendEnable;
		d3dBlendDesc.SrcBlend = blendTypeDic.at(description.srcBlend);
		d3dBlendDesc.DestBlend = blendTypeDic.at(description.destBlend);
		d3dBlendDesc.SrcBlendAlpha = blendTypeDic.at(description.srcBlendAlpha);
		d3dBlendDesc.DestBlendAlpha = blendTypeDic.at(description.destBlendAlpha);
		d3dBlendDesc.BlendOp = blendOpTypeDic.at(description.blendOp);
		d3dBlendDesc.BlendOpAlpha = blendOpTypeDic.at(description.blendOpAlpha);
		d3dBlendDesc.RenderTargetWriteMask = mask;
		return d3dBlendDesc;
	}

	void D3D11BlendState::bind() const
	{
		assert(blendState);
		assert(engineInfo.getContext());
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.getContext();
		d3dDeviceContext->OMSetBlendState(blendState, blendStateDescription.blendFactor, blendStateDescription.sampleMask);
	}

	void D3D11BlendState::unbind() const
	{

	}
}