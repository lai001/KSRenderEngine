#include <OpenGL/GLVertexArray.hpp>
#include <glad/glad.h>

namespace ks
{
	GLVertexArray::GLVertexArray()
		: RendererID(0)
	{
		glGenVertexArrays(1, &RendererID);
		bind();
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &RendererID);
	}

	int GLVertexArray::getRendererID()
	{
		return RendererID;
	}

	void GLVertexArray::bind() const
	{
		glBindVertexArray(RendererID);
	}

	void GLVertexArray::unbind()
	{
		glBindVertexArray(0);
	}
}