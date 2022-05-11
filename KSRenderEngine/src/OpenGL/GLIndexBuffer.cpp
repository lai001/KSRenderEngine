#include "OpenGL/GLIndexBuffer.hpp"
#include <assert.h>
#include <glad/glad.h>

namespace ks
{
	GLIndexBuffer::GLIndexBuffer(const unsigned int* Data, unsigned int Count)
		: Count(Count)
	{
		assert(Data);
		glGenBuffers(1, &RendererID);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(unsigned int), Data, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &RendererID);
	}

	unsigned int GLIndexBuffer::getCount() const
	{
		return Count;
	}

	void GLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
	}

	void GLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}