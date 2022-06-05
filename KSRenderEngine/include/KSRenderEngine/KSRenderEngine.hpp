#ifndef KSRenderEngine_hpp
#define KSRenderEngine_hpp

#include <Foundation/Foundation.hpp>
#include "Common/Common.hpp"
#include "Interface/Interface.hpp"
#include "OpenGL/GLRenderEngine.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{
	class RenderEngine
	{
	public:
		static IRenderEngine* create(const GLRenderEngineCreateInfo& createInfo);
		static IRenderEngine* create(const D3D11RenderEngineCreateInfo& createInfo);
	};

}

#endif // !KSRenderEngine_hpp