#include <OpenGL/GLVertexBuffer.hpp>
//#include "OpenGL/GLUtil.hpp"
#include <assert.h>
#include <glad/glad.h>

namespace ks
{
	GLVertexBuffer::GLVertexBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout)
		:layout(layout)
	{
		assert(data);
		glGenBuffers(1, &RendererID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &RendererID);
	}

	void GLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	}

	void GLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::applyLayout() const
	{
		std::unordered_map<VertexBufferElement::Type, int> dicf;
		std::unordered_map<VertexBufferElement::Type, int> dici;

		dici[VertexBufferElement::Type::byte] = GL_BYTE;
		dici[VertexBufferElement::Type::ubyte] = GL_UNSIGNED_BYTE;
		dici[VertexBufferElement::Type::int16] = GL_SHORT;
		dici[VertexBufferElement::Type::uint16] = GL_UNSIGNED_SHORT;
		dici[VertexBufferElement::Type::uint32] = GL_UNSIGNED_INT;
		dici[VertexBufferElement::Type::int32] = GL_INT;

		dicf[VertexBufferElement::Type::f32] = GL_FLOAT;
		dicf[VertexBufferElement::Type::f64] = GL_DOUBLE;

		const std::vector<VertexBufferElement> elements = layout.getElements();
		unsigned int Offset = 0;
		for (size_t i = 0; i < elements.size(); i++)
		{
			const VertexBufferElement element = elements[i];
			glEnableVertexAttribArray(i);

			if (dici.find(element.type) != dici.end())
			{
				glVertexAttribIPointer(i, element.count, dici.at(element.type), layout.getStride(), (const void*)Offset);
				Offset += element.count * VertexBufferElement::getSize(element.type);
			}
			else if (dicf.find(element.type) != dicf.end())
			{
				glVertexAttribPointer(i, element.count, dicf.at(element.type), element.isNormalized, layout.getStride(), (const void*)Offset);
				Offset += element.count * VertexBufferElement::getSize(element.type);
			}
			else
			{
				assert(false);
			}
		}
	}

	unsigned int GLVertexBuffer::getRendererID() const
	{
		return RendererID;
	}

	VertexBufferLayout GLVertexBuffer::getLayout() const
	{
		return layout;
	}
}