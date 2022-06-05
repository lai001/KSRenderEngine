#ifndef KSRenderEngine_OpenGL_GLRenderEngine
#define KSRenderEngine_OpenGL_GLRenderEngine

#include "Interface/RenderEngine.hpp"

namespace ks
{
	struct GLRenderEngineCreateInfo
	{

	};

	struct GLRenderEngineInfo
	{

	};

	class GLRenderEngine : public boost::noncopyable, public IRenderEngine
	{
	private:
		GLRenderEngineCreateInfo createInfo;

	public:
		explicit GLRenderEngine(const GLRenderEngineCreateInfo& createInfo);

		virtual IFrameBuffer * createFrameBuffer(const int width, const int height) override;

		virtual void readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer) override;

		virtual void enableDebug(const bool flag) override;

		virtual IShader * createShader(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource,
			const std::vector<UniformBufferInfo>& uniformBuffers,
			const std::vector<ShaderTexture2DInfo> texture2DInfos,
			const ks::VertexBufferLayout& layout) override;

		virtual IShader * createShader(const std::string & VertexShaderSource,
			const std::string & FragmentShaderSource) override;

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
		virtual ITexture2D * createTexture2D(const unsigned int width,
			const unsigned int height,
			const TextureFormat textureFormat, 
			const unsigned char * data) override;

		virtual void erase(IDeletable * deletable) override;

		virtual void addRenderCommand(std::function<void()> command) override;

		virtual std::function<void()> renderCommand() override;

		virtual void attachToCurrentThread() override;
	};
}

#endif // !KSRenderEngine_OpenGL_GLRenderEngine