#ifndef KSRENDERENGIIndexBuffer
#define KSRENDERENGIIndexBuffer

namespace ks
{
	class IIndexBuffer
	{
	public:
		enum class IndexDataType
		{
			uint16,
			uint32,
		};

	public:
		virtual ~IIndexBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual IIndexBuffer::IndexDataType getIndexDataType() noexcept = 0;

		static unsigned int getSizeOf(IIndexBuffer::IndexDataType indexDataType) noexcept;
	};
}

#endif // !KSRENDERENGIIndexBuffer