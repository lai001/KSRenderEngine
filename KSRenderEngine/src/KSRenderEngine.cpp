#include "KSRenderEngine.hpp"

namespace ks
{
	IRenderEngine * RenderEngine::create(const GLRenderEngineCreateInfo& createInfo)
	{
		GLRenderEngine* engine = new GLRenderEngine(createInfo);
		return engine;
	}

	IRenderEngine * RenderEngine::create(const D3D11RenderEngineCreateInfo& createInfo)
	{
		D3D11RenderEngine* engine = new D3D11RenderEngine(createInfo);
		return engine;
	}
}
