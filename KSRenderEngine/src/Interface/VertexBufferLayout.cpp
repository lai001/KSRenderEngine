#include "Interface/VertexBufferLayout.hpp"

namespace ks
{
	VertexBufferLayout & VertexBufferLayout::f32(const unsigned int count, bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::f32, count, isNormalized));
	}

	VertexBufferLayout & VertexBufferLayout::uint32(const unsigned int count, bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::uint32, count, isNormalized));
	}

	VertexBufferLayout & VertexBufferLayout::ubyte(const unsigned int count, bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::ubyte, count, isNormalized));
	}

	VertexBufferLayout & VertexBufferLayout::int32(const unsigned int count, bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::int32, count, isNormalized));
	}

	VertexBufferLayout & VertexBufferLayout::f32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::f32, count, isNormalized, semantics, semanticsIndex));
	}

	VertexBufferLayout & VertexBufferLayout::uint32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::uint32, count, isNormalized, semantics, semanticsIndex));
	}

	VertexBufferLayout & VertexBufferLayout::ubyte(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::ubyte, count, isNormalized, semantics, semanticsIndex));
	}

	VertexBufferLayout & VertexBufferLayout::int32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		return element(VertexBufferElement(VertexBufferElement::Type::int32, count, isNormalized, semantics, semanticsIndex));
	}

	VertexBufferLayout & VertexBufferLayout::element(const VertexBufferElement & element)
	{
		elements.push_back(element);
		stride += element.count * VertexBufferElement::getSize(element.type);
		return *this;
	}

	std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
	{
		return elements;
	}

	unsigned int VertexBufferLayout::getStride() const
	{
		return stride;
	}
}