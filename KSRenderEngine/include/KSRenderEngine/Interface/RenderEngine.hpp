#ifndef KSRenderEngine_Interface_IRenderEngine
#define KSRenderEngine_Interface_IRenderEngine

#include "FrameBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "VertexBuffer.hpp"
#include "RenderBuffer.hpp"
#include "BlendState.hpp"
#include "RasterizerState.hpp"
#include "DepthStencilState.hpp"

#include "Common/VertexBufferLayout.hpp"
#include "Common/Uniform.hpp"
#include "Common/Deletable.hpp"
#include "Common/Texture2DDescription.hpp"
#include "Common/BlendStateDescription.hpp"
#include "Common/DepthStencilStateDescription.hpp"
#include "Common/RasterizerStateDescription.hpp"

namespace ks
{
	class IRenderEngine
	{
	public:
		virtual IFrameBuffer* createFrameBuffer(const int width, const int height) = 0;
		virtual IShader* createShader(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource,
			const std::vector<UniformBufferInfo>& uniformBuffers,
			const std::vector<ShaderTexture2DInfo> texture2DInfos,
			const ks::VertexBufferLayout& layout) = 0;

		virtual IShader* createShader(const std::string& VertexShaderSource,
			const std::string& FragmentShaderSource) = 0;

		//virtual IVertexBuffer* createVertexBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout) = 0;
		//virtual IIndexBuffer* createIndexBuffer(const void* data, const unsigned int count, const unsigned int stride) = 0;
		virtual IRenderBuffer* createRenderBuffer(const void * vertexBuffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const VertexBufferLayout & layout,
			const void * indexBufferData,
			const unsigned int indexCount,
			const IIndexBuffer::IndexDataType type) = 0;
		virtual IBlendState* createBlendState(const BlendStateDescription::Addition& addition, 
			const BlendStateDescription& blendStateDescription) = 0;
		virtual IRasterizerState* createRasterizerState(const RasterizerStateDescription& rasterizerStateDescription) = 0;
		virtual IDepthStencilState* createDepthStencilState(const DepthStencilStateDescription& depthStencilStateDescription) = 0;
		virtual ITexture2D* createTexture2D(const unsigned int width, 
			const unsigned int height,
			const TextureFormat textureFormat,
			const unsigned char* data) = 0;

		virtual void readTexture(const IFrameBuffer* frameBuffer, ks::PixelBuffer& pixelBuffer) = 0;

		virtual void erase(IDeletable* deletable) = 0;

		virtual void enableDebug(const bool flag) = 0;
	};
}

#endif // !KSRenderEngine_Interface_IRenderEngine