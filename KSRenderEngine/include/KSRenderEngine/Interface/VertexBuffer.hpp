#ifndef KSRENDERENGIVertexBuffer
#define KSRENDERENGIVertexBuffer

namespace ks
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRENDERENGIVertexBuffer