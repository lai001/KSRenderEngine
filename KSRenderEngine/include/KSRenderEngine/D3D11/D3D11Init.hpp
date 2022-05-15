#ifndef KSRenderEngine_D3D11_D3D11Init_hpp
#define KSRenderEngine_D3D11_D3D11Init_hpp

#ifndef _WIN32
#error
#endif // !_WIN32
#include <d3d11.h>

namespace ks
{
	struct D3D11Data
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
	};

	D3D11Data D3D11Init();
}

#endif // !KSRenderEngine_D3D11_D3D11Init_hpp