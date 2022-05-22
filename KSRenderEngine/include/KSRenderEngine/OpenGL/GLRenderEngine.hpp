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
		explicit GLRenderEngine(GLRenderEngineCreateInfo createInfo);

		IFrameBuffer * createFrameBuffer(const int width, const int height) override;

		void readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer) override;

		void enableDebug(const bool flag) override;

		IShader * createShader(const std::string& VertexShaderSource,
			const std::string& FragmentShaderSource,
			const std::vector<UniformInfo>& createInfos,
			const std::vector<ShaderTexture2DInfo> texture2DInfo,
			const ks::VertexBufferLayout& layout) override;

		IShader * createShader(const std::string & VertexShaderSource,
			const std::string & FragmentShaderSource) override;

		IRenderBuffer * createRenderBuffer(const void * vertexBuffer, 
			const unsigned int vertexCount, 
			const unsigned int vertexStride,
			const VertexBufferLayout & layout, 
			const void * indexBufferData, 
			const unsigned int indexCount, 
			const IIndexBuffer::IndexDataType type) override;

		IBlendState * createBlendState(const BlendStateDescription::Addition& addition,
			const BlendStateDescription& blendStateDescription) override;
		IRasterizerState * createRasterizerState(const ks::RasterizerStateDescription &rasterizerStateDescription) override;
		IDepthStencilState * createDepthStencilState(const ks::DepthStencilStateDescription &depthStencilStateDescription) override;
		ITexture2D * createTexture2D(const unsigned int width, 
			const unsigned int height,
			const TextureFormat textureFormat, 
			const unsigned char * data) override;

		void erase(IDeletable * deletable) override;
	};
}

#endif // !KSRenderEngine_OpenGL_GLRenderEngine