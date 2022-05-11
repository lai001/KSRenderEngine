#ifndef KSRENDERENGIIndexBuffer
#define KSRENDERENGIIndexBuffer

namespace ks
{
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRENDERENGIIndexBuffer