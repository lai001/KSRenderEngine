#ifndef KSRenderEngine_D3D11_D3D11RasterizerState_hpp
#define KSRenderEngine_D3D11_D3D11RasterizerState_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include <Foundation/Foundation.hpp>
#include <Interface/RasterizerState.hpp>
#include "Common/RasterizerStateDescription.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{
	class D3D11RasterizerState:public noncopyable, public IRasterizerState
	{
	public:
		D3D11RasterizerState(const RasterizerStateDescription& rasterizerStateDescription,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11RasterizerState();
		void bind() const override;
		void unbind() const override;

	private:
		D3D11RenderEngineInfo engineInfo;
		ID3D11RasterizerState* rasterizerState = nullptr;
		RasterizerStateDescription rasterizerStateDescription;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11RasterizerState_hpp