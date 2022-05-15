#ifndef KSRenderEngine_D3D11_D3D11DepthStencilState_hpp
#define KSRenderEngine_D3D11_D3D11DepthStencilState_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include <Foundation/Foundation.hpp>
#include "Common/DepthStencilStateDescription.hpp"
#include "Interface/DepthStencilState.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{
	class D3D11DepthStencilState :public boost::noncopyable, public IDepthStencilState
	{
	public:
		D3D11DepthStencilState(const DepthStencilStateDescription& depthStencilStateDescription,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11DepthStencilState();
		void bind() const override;
		void unbind() const override;

	private:
		ID3D11DepthStencilState* depthStencilState = nullptr;
		D3D11RenderEngineInfo engineInfo;
		DepthStencilStateDescription depthStencilStateDescription;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11DepthStencilState_hpp