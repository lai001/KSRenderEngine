#include "OpenGL/GLIndexBuffer.hpp"
#include <assert.h>
#include <glad/glad.h>

namespace ks
{
	GLIndexBuffer::GLIndexBuffer(const void* data, unsigned int count, const IndexDataType indexDataType)
		: count(count), indexDataType(indexDataType)
	{
		assert(data);
		glGenBuffers(1, &rendererID);
		bind();
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * IIndexBuffer::getSizeOf(indexDataType), data, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	unsigned int GLIndexBuffer::getCount() const
	{
		return count;
	}

	void GLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	}

	void GLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IIndexBuffer::IndexDataType GLIndexBuffer::getIndexDataType() noexcept
	{
		return indexDataType;
	}
}