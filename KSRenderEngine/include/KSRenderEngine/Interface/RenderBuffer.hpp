#ifndef KSRenderEngineRenderBuffer
#define KSRenderEngineRenderBuffer

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "FrameBuffer.hpp"
#include "BlendState.hpp"
#include "DepthStencilState.hpp"
#include "RasterizerState.hpp"

#include "Common/PrimitiveTopologyType.hpp"
#include "Common/Deletable.hpp"

namespace ks
{
	class IRenderBuffer : public IDeletable
	{
	public:
		virtual ~IRenderBuffer() = 0 { };
		virtual void commit(const IFrameBuffer& frameBuffer) = 0;
		//virtual void setVertexBuffer(const IVertexBuffer& vertexBuffer) = 0;
		//virtual void setIndexBuffer(const IIndexBuffer& indexBuffer) = 0;
		virtual void setClearBufferFlags(const ks::ClearBufferFlags& flags) = 0;
		virtual void setClearColor(const glm::vec4& clearColor) = 0;
		virtual void setViewport(const int x, const int y, const int width, const int height) = 0;
		virtual void setShader(const IShader& shader) = 0;
		virtual void setBlendState(const IBlendState& blendState) = 0;
		virtual void setDepthStencilState(const IDepthStencilState& depthStencilState) = 0;
		virtual void setRasterizerState(const IRasterizerState& rasterizerState) = 0;
		virtual void setPrimitiveTopologyType(const PrimitiveTopologyType& primitiveTopologyType) = 0;
	};
}

#endif // !KSRenderEngineRenderBuffer