#include "KSRenderEngine.hpp"

namespace ks
{
	IRenderEngine * RenderEngine::create(GLRenderEngineCreateInfo createInfo)
	{
		GLRenderEngine* engine = new GLRenderEngine(createInfo);
		return engine;
	}

	IRenderEngine * RenderEngine::create(D3D11RenderEngineCreateInfo createInfo)
	{
		D3D11RenderEngine* engine = new D3D11RenderEngine(createInfo);
		return engine;
	}
}
