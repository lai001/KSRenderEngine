#include "Interface/IndexBuffer.hpp"
#include <unordered_map>
#include <assert.h>

namespace ks
{
	unsigned int IIndexBuffer::getSizeOf(IIndexBuffer::IndexDataType indexDataType) noexcept
	{
		std::unordered_map<IndexDataType, unsigned int> size;
		size[IndexDataType::uint16] = 2;
		size[IndexDataType::uint32] = 4;
		assert(size.end() != size.find(indexDataType));
		return size.at(indexDataType);
	}
}
