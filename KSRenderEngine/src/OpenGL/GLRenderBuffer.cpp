#include "OpenGL/GLRenderBuffer.hpp"
#include <assert.h>

namespace ks
{
	GLRenderBuffer::GLRenderBuffer(const void * data, const unsigned int size, const VertexBufferLayout & layout, const unsigned int * Data, const unsigned int Count)
	{
		vertexArray = std::make_unique<GLVertexArray>();
		vertextBuffer = std::make_unique<GLVertexBuffer>(data, size, layout);
		indexBuffer = std::make_unique<GLIndexBuffer>(Data, Count);
		vertextBuffer->applyLayout();
		vertexArray->unbind();
	}

	void GLRenderBuffer::commit()
	{
		assert(shader);
		assert(vertextBuffer);
		assert(indexBuffer);
		shader->bind();
		vertexArray->bind();
		if (frameBuffer)
		{
			frameBuffer->bind();
		}
		glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
		shader->unbind();
		vertexArray->unbind();
		if (frameBuffer)
		{
			frameBuffer->unbind();
		}
	}

	void GLRenderBuffer::setShader(const IShader & shader)
	{
		this->shader = dynamic_cast<const GLShader*>(&shader);
	}

	void GLRenderBuffer::setFrameBuffer(const IFrameBuffer & frameBuffer)
	{
		this->frameBuffer = &frameBuffer;
	}
}
