#ifndef KSRenderEngine_D3D11_D3D11RenderEngineInfo_hpp
#define KSRenderEngine_D3D11_D3D11RenderEngineInfo_hpp

#ifndef _WIN32
#error
#endif // !_WIN32
#include "Platform/windows.hpp"
#include "Interface/RenderEngine.hpp"

namespace ks
{
	struct D3D11RenderEngineInfo
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* immediateContext = nullptr;
		ID3D11DeviceContext* deferredContext = nullptr;
		IRenderEngine* renderEngine = nullptr;

		ID3D11DeviceContext* getContext() const noexcept;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RenderEngineInfo_hpp
