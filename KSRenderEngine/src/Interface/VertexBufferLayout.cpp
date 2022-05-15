#include "Interface/VertexBufferLayout.hpp"


namespace ks
{
	VertexBufferLayout & VertexBufferLayout::f32(const unsigned int count, bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::f32, count, isNormalized));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::f32);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::uint32(const unsigned int count, bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::uint32, count, isNormalized));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::uint32);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::ubyte(const unsigned int count, bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::ubyte, count, isNormalized));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::ubyte);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::int32(const unsigned int count, bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::int32, count, isNormalized));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::int32);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::f32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::f32, count, isNormalized, semantics, semanticsIndex));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::f32);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::uint32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::uint32, count, isNormalized, semantics, semanticsIndex));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::uint32);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::ubyte(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::ubyte, count, isNormalized, semantics, semanticsIndex));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::ubyte);
		return *this;
	}

	VertexBufferLayout & VertexBufferLayout::int32(const unsigned int count,
		const VertexBufferElement::Semantics semantics,
		const unsigned int semanticsIndex,
		bool isNormalized)
	{
		elements.push_back(VertexBufferElement(VertexBufferElement::Type::int32, count, isNormalized, semantics, semanticsIndex));
		stride += count * VertexBufferElement::getSize(VertexBufferElement::Type::int32);
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