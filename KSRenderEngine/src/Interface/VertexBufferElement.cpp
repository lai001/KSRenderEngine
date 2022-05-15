#include "Interface/VertexBufferElement.hpp"
#include <unordered_map>
#include <assert.h>

namespace ks
{
	unsigned int VertexBufferElement::getSize(const VertexBufferElement::Type & type)
	{
		std::unordered_map<VertexBufferElement::Type, unsigned int> dic;
		dic[VertexBufferElement::Type::byte] = 1;
		dic[VertexBufferElement::Type::ubyte] = 1;
		dic[VertexBufferElement::Type::int16] = 2;
		dic[VertexBufferElement::Type::uint16] = 2;
		dic[VertexBufferElement::Type::uint32] = 4;
		dic[VertexBufferElement::Type::int32] = 4;
		dic[VertexBufferElement::Type::f32] = 4;
		dic[VertexBufferElement::Type::f64] = 8;
		assert(dic.end() != dic.find(type));
		return dic.at(type);
	}

	VertexBufferElement::VertexBufferElement(const VertexBufferElement::Type type,
		const unsigned int count,
		const bool isNormalized,
		const Semantics semantics,
		unsigned int semanticsIndex)
		:type(type), 
		isNormalized(isNormalized),
		count(count), 
		semantics(semantics), 
		semanticsIndex(semanticsIndex)
	{

	}

	VertexBufferElement::VertexBufferElement(const VertexBufferElement::Type type,
		const unsigned int count,
		const bool isNormalized)
		: type(type),
		isNormalized(isNormalized), 
		count(count), 
		semantics(Semantics::unknow),
		semanticsIndex(0)
	{

	}

}