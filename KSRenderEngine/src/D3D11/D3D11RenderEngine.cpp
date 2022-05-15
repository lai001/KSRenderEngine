#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11Shader.hpp"
#include "D3D11/D3D11FrameBuffer.hpp"
#include "D3D11/D3D11RenderBuffer.hpp"
#include "D3D11/D3D11BlendState.hpp"
#include "D3D11/D3D11DepthStencilState.hpp"
#include "D3D11/D3D11RasterizerState.hpp"


namespace ks
{
	D3D11RenderEngine::D3D11RenderEngine(D3D11RenderEngineCreateInfo createInfo)
		:createInfo(createInfo)
	{
		engineInfo.context = createInfo.context;
		engineInfo.device = createInfo.device;
	}

	IFrameBuffer * D3D11RenderEngine::createFrameBuffer(const int width, const int height)
	{
		D3D11FrameBuffer* framBuffer = new D3D11FrameBuffer(width, height, engineInfo);
		return framBuffer;
	}

	IShader * D3D11RenderEngine::createShader(const std::string & VertexShaderSource,
		const std::string & FragmentShaderSource,
		const std::vector<UniformInfo>& createInfos,
		const ks::VertexBufferLayout & layout)
	{
		D3D11Shader* shader = D3D11Shader::create(VertexShaderSource,
			FragmentShaderSource,
			createInfos,
			layout,
			engineInfo);
		return shader;
	}

	IRenderBuffer * D3D11RenderEngine::createRenderBuffer(const void * vertexBuffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const VertexBufferLayout & layout,
		const void * indexBufferData,
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type)
	{
		D3D11RenderBuffer* renderBuffer = new D3D11RenderBuffer(vertexBuffer, vertexCount, vertexStride,
			indexBufferData, indexCount, type,
			engineInfo);
		return renderBuffer;
	}

	void D3D11RenderEngine::readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer)
	{
		assert(frameBuffer);
		const D3D11FrameBuffer* d3d11FrameBuffer = dynamic_cast<const D3D11FrameBuffer*>(frameBuffer);
		assert(d3d11FrameBuffer);
		d3d11FrameBuffer->read(pixelBuffer);
	}

	void D3D11RenderEngine::erase(IFrameBuffer * ptr)
	{
		delete ptr;
	}

	void D3D11RenderEngine::erase(IShader * ptr)
	{
		delete ptr;
	}

	void D3D11RenderEngine::erase(IRenderBuffer * ptr)
	{
		delete ptr;
	}

	void D3D11RenderEngine::erase(IBlendState * ptr)
	{
		delete ptr;
	}

	void D3D11RenderEngine::erase(IRasterizerState * ptr)
	{
		delete ptr;
	}

	void D3D11RenderEngine::erase(IDepthStencilState * ptr)
	{
		delete ptr;
	}

	ks::IBlendState * D3D11RenderEngine::createBlendState(const BlendStateDescription::Addition& addition,
		const BlendStateDescription& blendStateDescription)
	{
		D3D11BlendState* state = new D3D11BlendState(addition, blendStateDescription, engineInfo);
		return state;
	}

	ks::IRasterizerState * D3D11RenderEngine::createRasterizerState(const ks::RasterizerStateDescription &rasterizerStateDescription)
	{
		D3D11RasterizerState* state = new D3D11RasterizerState(rasterizerStateDescription, engineInfo);
		return state;
	}

	ks::IDepthStencilState * D3D11RenderEngine::createDepthStencilState(const ks::DepthStencilStateDescription &depthStencilStateDescription)
	{
		D3D11DepthStencilState* state = new D3D11DepthStencilState(depthStencilStateDescription, engineInfo);
		return state;
	}

	void D3D11RenderEngine::enableDebug(const bool flag)
	{
	}
}