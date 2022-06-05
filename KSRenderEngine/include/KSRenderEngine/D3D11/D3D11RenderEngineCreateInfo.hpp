#ifndef KSRenderEngine_D3D11_D3D11RenderEngineCreateInfo_hpp
#define KSRenderEngine_D3D11_D3D11RenderEngineCreateInfo_hpp

#ifndef _WIN32
#error
#endif // !_WIN32
#include "Platform/windows.hpp"

namespace ks
{
	struct D3D11RenderEngineCreateInfo
	{
		struct NativeData
		{
			ID3D11Device* device = nullptr;
			ID3D11DeviceContext* context = nullptr;
		};
		NativeData* data = nullptr;
		bool isDeferredContextEnable = false;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RenderEngineCreateInfo_hpp
