#ifndef KSRenderEngine_D3D11_D3D11RenderTargetView_hpp
#define KSRenderEngine_D3D11_D3D11RenderTargetView_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include "Platform/windows.hpp"
#include <Foundation/Foundation.hpp>
#include "Common/Texture2DDescription.hpp"
#include "Interface/RenderTargetView.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11Texture2D.hpp"

namespace ks
{
	class D3D11RenderTargetView: public noncopyable, public IRenderTargetView
	{
	public:
		D3D11RenderTargetView(const D3D11Texture2D& d3d11Texture2D, const D3D11RenderEngineInfo& engineInfo);
		~D3D11RenderTargetView();
		ID3D11RenderTargetView* getNativeView() const;
		void bind() const override;
		void unbind() const override;

	private:
		ID3D11RenderTargetView* renderTargetView = nullptr;
		D3D11RenderEngineInfo engineInfo;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RenderTargetView_hpp