#include "D3D11/D3D11RenderTargetView.hpp"


namespace ks
{
	D3D11RenderTargetView::D3D11RenderTargetView(const D3D11Texture2D& d3d11Texture2D, const D3D11RenderEngineInfo& engineInfo)
		:engineInfo(engineInfo)
	{
		assert(engineInfo.context);
		assert(engineInfo.device);
		assert(d3d11Texture2D.getNativeTexture2D());
		HRESULT status = S_OK;
		ID3D11Device *d3d11Device = engineInfo.device;

		D3D11_TEX2D_RTV rtv;
		rtv.MipSlice = 0;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = d3d11Texture2D.getNativeTextureFormat();
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D = rtv;
		status = d3d11Device->CreateRenderTargetView(d3d11Texture2D.getNativeTexture2D(), &renderTargetViewDesc, &renderTargetView);
		assert(status == S_OK);
	}

	D3D11RenderTargetView::~D3D11RenderTargetView()
	{
		assert(renderTargetView);
		renderTargetView->Release();
	}

	ID3D11RenderTargetView * D3D11RenderTargetView::getNativeView() const
	{
		return renderTargetView;
	}
}


