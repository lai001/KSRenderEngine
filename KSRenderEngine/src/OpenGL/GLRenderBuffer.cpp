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

	void GLRenderBuffer::commit(const IFrameBuffer& frameBuffer)
	{
		assert(shader);
		assert(vertextBuffer);
		assert(indexBuffer);
		frameBuffer.bind();
		shader->bind();
		vertexArray->bind();
		viewport();
		clearColor();
		clear();

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
		frameBuffer.unbind();
	}

	void GLRenderBuffer::setClearBufferFlags(const ks::ClearBufferFlags & flags)
	{
		clearBufferFlags = flags;
	}

	void GLRenderBuffer::setClearColor(const glm::vec4 & clearColor)
	{
		_clearColor = clearColor;
	}

	void GLRenderBuffer::setViewport(const int x, const int y, const int width, const int height)
	{
		_viewport = glm::ivec4(x, y, width, height);
	}

	void GLRenderBuffer::clear() const
	{
		if (clearBufferFlags.isContains(ks::ClearBufferFlags::color))
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		if (clearBufferFlags.isContains(ks::ClearBufferFlags::depth))
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		if (clearBufferFlags.isContains(ks::ClearBufferFlags::stencil))
		{
			glClear(GL_STENCIL_BUFFER_BIT);
		}
	}

	void GLRenderBuffer::clearColor() const
	{
		glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	}

	void GLRenderBuffer::viewport() const
	{
		glViewport(_viewport.x, _viewport.y, _viewport.z, _viewport.w);

	}

	void GLRenderBuffer::setShader(const IShader & shader)
	{
		this->shader = dynamic_cast<const GLShader*>(&shader);
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