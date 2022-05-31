#ifndef KSRenderEngine_Interface_IndexBuffer_hpp
#define KSRenderEngine_Interface_IndexBuffer_hpp
#include "Common/Deletable.hpp"

namespace ks
{
	class IIndexBuffer : public IDeletable
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

#endif // !KSRenderEngine_Interface_IndexBuffer_hpp