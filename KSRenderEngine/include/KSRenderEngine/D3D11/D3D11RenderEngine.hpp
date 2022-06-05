
#ifndef KSRenderEngine_D3D11_D3D11RenderEngine
#define KSRenderEngine_D3D11_D3D11RenderEngine

#ifndef _WIN32
#error
#endif // !_WIN32
#include <mutex>
#include "Interface/RenderEngine.hpp"
#include "Platform/windows.hpp"
#include "D3D11RenderEngineCreateInfo.hpp"
#include "D3D11RenderEngineInfo.hpp"
#include "D3D11Init.hpp"

namespace ks
{
	class D3D11RenderEngine : public noncopyable, public IRenderEngine
	{
	private:
		D3D11RenderEngineCreateInfo createInfo;
		D3D11RenderEngineInfo engineInfo;

		std::unique_ptr<D3D11Window> window;

		std::vector<std::function<void()>> commands;
		std::mutex commandMutex;

	public:
		explicit D3D11RenderEngine(const D3D11RenderEngineCreateInfo& createInfo);

		virtual IFrameBuffer * createFrameBuffer(const int width, const int height) override;

		virtual IRenderBuffer * createRenderBuffer(const void * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const IShader & shader,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type) override;

		virtual IBlendState * createBlendState(const BlendStateDescription::Addition& addition,
			const BlendStateDescription& blendStateDescription) override;
		virtual IRasterizerState * createRasterizerState(const ks::RasterizerStateDescription &rasterizerStateDescription) override;
		virtual IDepthStencilState * createDepthStencilState(const ks::DepthStencilStateDescription &depthStencilStateDescription) override;

		virtual IShader * createShader(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource,
			const std::vector<UniformBufferInfo>& uniformBuffers,
			const std::vector<ShaderTexture2DInfo> texture2DInfos,
			const ks::VertexBufferLayout& layout) override;

		virtual IShader * createShader(const std::string & VertexShaderSource,
			const std::string & FragmentShaderSource) override;

		virtual void readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer) override;

		virtual void enableDebug(const bool flag) override;

		virtual ITexture2D * createTexture2D(const unsigned int width,
			const unsigned int height, 
			const TextureFormat textureFormat, 
			const unsigned char * data) override;

		virtual void erase(IDeletable * deletable) override;

		virtual void addRenderCommand(std::function<void()> task) override;

		virtual std::function<void()> renderCommand() override;

		virtual void attachToCurrentThread() override;
	};

}

#endif // !KSRenderEngine_D3D11_D3D11RenderEngine
