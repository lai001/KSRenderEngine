#ifndef KSRenderEngine_Interface_VertexBuffer_hpp
#define KSRenderEngine_Interface_VertexBuffer_hpp
#include "Common/Deletable.hpp"

namespace ks
{
	class IVertexBuffer : public IDeletable
	{
	public:
		virtual ~IVertexBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_VertexBuffer_hpp