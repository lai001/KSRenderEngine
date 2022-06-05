#ifndef KSRenderEngine_D3D11_D3D11BlendState_hpp
#define KSRenderEngine_D3D11_D3D11BlendState_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <Foundation/Foundation.hpp>
#include "Platform/windows.hpp"
#include "Interface/BlendState.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "Common/BlendStateDescription.hpp"

namespace ks
{
	class D3D11BlendState : public noncopyable, public IBlendState
	{
	public:
		explicit D3D11BlendState(const BlendStateDescription::Addition& addition,
			const BlendStateDescription& description,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11BlendState();

		static D3D11_RENDER_TARGET_BLEND_DESC toD3DBlendDesc(const BlendStateDescription& description);
		void bind() const override;
		void unbind() const override;

	private:
		D3D11RenderEngineInfo engineInfo;
		ID3D11BlendState* blendState = nullptr;
		BlendStateDescription blendStateDescription;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11BlendState_hpp