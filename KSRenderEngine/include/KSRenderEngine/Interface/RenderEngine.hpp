#ifndef KSRenderEngine_Interface_IRenderEngine
#define KSRenderEngine_Interface_IRenderEngine

#include "FrameBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Uniform.hpp"
#include "RenderBuffer.hpp"
#include "BlendState.hpp"
#include "RasterizerState.hpp"
#include "DepthStencilState.hpp"

#include "Common/BlendStateDescription.hpp"
#include "Common/DepthStencilStateDescription.hpp"
#include "Common/RasterizerStateDescription.hpp"

namespace ks
{
	class IRenderEngine
	{
	public:
		virtual IFrameBuffer* createFrameBuffer(const int width, const int height) = 0;
		virtual IShader* createShader(const std::string& VertexShaderSource,
			const std::string& FragmentShaderSource, 
			const std::vector<UniformInfo>& createInfos,
			const ks::VertexBufferLayout& layout) = 0;
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

		virtual void readTexture(const IFrameBuffer* frameBuffer, ks::PixelBuffer& pixelBuffer) = 0;

		virtual void erase(IFrameBuffer* ptr) = 0;
		virtual void erase(IShader* ptr) = 0;
		virtual void erase(IRenderBuffer* ptr) = 0;
		virtual void erase(IBlendState* ptr) = 0;
		virtual void erase(IRasterizerState* ptr) = 0;
		virtual void erase(IDepthStencilState* ptr) = 0;

		virtual void enableDebug(const bool flag) = 0;
	};
}

#endif // !KSRenderEngine_Interface_IRenderEngine