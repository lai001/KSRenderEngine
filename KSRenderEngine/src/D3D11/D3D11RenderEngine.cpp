#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11Shader.hpp"
#include "D3D11/D3D11FrameBuffer.hpp"
#include "D3D11/D3D11RenderBuffer.hpp"
#include "D3D11/D3D11BlendState.hpp"
#include "D3D11/D3D11DepthStencilState.hpp"
#include "D3D11/D3D11RasterizerState.hpp"

namespace ks
{
	D3D11RenderEngine::D3D11RenderEngine(const D3D11RenderEngineCreateInfo& createInfo)
		:createInfo(createInfo)
	{
		D3D11RenderEngineCreateInfo::NativeData nativeData;

		if (createInfo.data)
		{
			nativeData = *createInfo.data;
		}
		else
		{
			D3D11Window::Configuration cfg;
			window = std::make_unique<D3D11Window>(cfg);
			nativeData.context = window->getDeviceContext();
			nativeData.device = window->getDevice();
		}
		assert(nativeData.device);
		assert(nativeData.context);
		assert(nativeData.context->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE);
		engineInfo.immediateContext = nativeData.context;
		engineInfo.device = nativeData.device;
		engineInfo.renderEngine = this;
	}

	IFrameBuffer * D3D11RenderEngine::createFrameBuffer(const int width, const int height)
	{
		D3D11FrameBuffer* framBuffer = new D3D11FrameBuffer(width, height, engineInfo);
		return framBuffer;
	}

	IShader * D3D11RenderEngine::createShader(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource,
		const std::vector<UniformBufferInfo>& uniformBuffers,
		const std::vector<ShaderTexture2DInfo> texture2DInfos,
		const ks::VertexBufferLayout& layout)
	{
		D3D11Shader* shader = D3D11Shader::create(vertexShaderSource,
			fragmentShaderSource,
			uniformBuffers,
			texture2DInfos,
			layout,
			engineInfo);
		return shader;
	}

	IShader * D3D11RenderEngine::createShader(const std::string & VertexShaderSource, const std::string & FragmentShaderSource)
	{
		D3D11Shader* shader = D3D11Shader::create(VertexShaderSource,
			FragmentShaderSource,
			engineInfo);
		return shader;
	}

	IRenderBuffer * D3D11RenderEngine::createRenderBuffer(const void * vertexBuffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const IShader & shader,
		const void * indexBufferData,
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type)
	{
		D3D11RenderBuffer* renderBuffer = new D3D11RenderBuffer(vertexBuffer, vertexCount, vertexStride,
			shader,
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

	ITexture2D * D3D11RenderEngine::createTexture2D(const unsigned int width,
		const unsigned int height,
		const TextureFormat textureFormat,
		const unsigned char * data)
	{
		assert(textureFormat == ks::TextureFormat::R8G8B8A8_UNORM);
		return D3D11Texture2D::create(width, height, textureFormat, data, engineInfo);
	}

	void D3D11RenderEngine::erase(IDeletable * deletable)
	{
		delete deletable;
	}

	void D3D11RenderEngine::addRenderCommand(std::function<void()> command)
	{
		std::lock_guard<std::mutex> lock(commandMutex);
		commands.push_back(command);
	}

	std::function<void()> D3D11RenderEngine::renderCommand()
	{
		std::lock_guard<std::mutex> lock(commandMutex);
		if (commands.empty() == false)
		{
			std::function<void()> command = commands.at(0);
			commands.erase(commands.begin());
			return command;
		}
		return std::function<void()>();
	}

	void D3D11RenderEngine::attachToCurrentThread()
	{
		if (createInfo.isDeferredContextEnable)
		{
			if (engineInfo.deferredContext)
			{
				assert(false);
			}
			HRESULT status = createInfo.data->device->CreateDeferredContext(0, &engineInfo.deferredContext);
			assert(SUCCEEDED(status));
		}
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

namespace ks
{
	ID3D11DeviceContext * D3D11RenderEngineInfo::getContext() const noexcept
	{
		if (deferredContext)
		{
			return deferredContext;
		}
		else
		{
			assert(immediateContext);
			return immediateContext;
		}
	}
}