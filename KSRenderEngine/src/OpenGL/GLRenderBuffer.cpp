#include "OpenGL/GLRenderBuffer.hpp"
#include <assert.h>

namespace ks
{
	GLRenderBuffer::GLRenderBuffer(const void * vertexBuffer,
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const VertexBufferLayout & layout,
		const void * indexBufferData,
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type)
	{
		vertexArray = std::make_unique<GLVertexArray>();
		vertextBuffer = std::make_unique<GLVertexBuffer>(vertexBuffer, vertexCount, vertexStride, layout);
		indexBuffer = std::make_unique<GLIndexBuffer>(indexBufferData, indexCount, type);
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
		IIndexBuffer::IndexDataType type = indexBuffer->getIndexDataType();
		if (type == IIndexBuffer::IndexDataType::uint16)
		{
			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_SHORT, 0);
		}
		else if (type == IIndexBuffer::IndexDataType::uint32)
		{
			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			assert(false);
		}

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

	void GLRenderBuffer::setBlendState(const IBlendState & blendState)
	{
	}

	void GLRenderBuffer::setDepthStencilState(const IDepthStencilState & depthStencilState)
	{
	}

	void GLRenderBuffer::setRasterizerState(const IRasterizerState & rasterizerState)
	{
	}

	void GLRenderBuffer::setPrimitiveTopologyType(const PrimitiveTopologyType & primitiveTopologyType)
	{
	}
}
