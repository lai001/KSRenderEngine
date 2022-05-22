#ifndef KSRENDERENGIVertexBuffer
#define KSRENDERENGIVertexBuffer
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

#endif // !KSRENDERENGIVertexBuffer