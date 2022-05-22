
#ifndef KSRenderEngine_D3D11_D3D11RenderEngine
#define KSRenderEngine_D3D11_D3D11RenderEngine

#ifndef _WIN32
#error
#endif // !_WIN32

#include "Interface/RenderEngine.hpp"
#include <d3d11.h>

namespace ks
{
	struct D3D11RenderEngineCreateInfo
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
	};

	struct D3D11RenderEngineInfo
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
	};

	class D3D11RenderEngine : public boost::noncopyable, public IRenderEngine
	{
	private:
		D3D11RenderEngineCreateInfo createInfo;
		D3D11RenderEngineInfo engineInfo;

	public:
		explicit D3D11RenderEngine(D3D11RenderEngineCreateInfo createInfo);

		IFrameBuffer * createFrameBuffer(const int width, const int height) override;

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

		IShader * createShader(const std::string& VertexShaderSource,
			const std::string& FragmentShaderSource,
			const std::vector<UniformInfo>& createInfos,
			const std::vector<ShaderTexture2DInfo> texture2DInfo,
			const ks::VertexBufferLayout& layout) override;

		IShader * createShader(const std::string & VertexShaderSource,
			const std::string & FragmentShaderSource) override;

		void readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer) override;

		void enableDebug(const bool flag) override;

		ITexture2D * createTexture2D(const unsigned int width,
			const unsigned int height, 
			const TextureFormat textureFormat, 
			const unsigned char * data) override;

		void erase(IDeletable * deletable) override;
	};

}

#endif // !KSRenderEngine_D3D11_D3D11RenderEngine
