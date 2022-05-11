#include "OpenGL/GLFrameBuffer.hpp"

namespace ks
{

	GLFrameBuffer::GLFrameBuffer(const int width, const int height)
		:width(width), height(height), _viewport(glm::ivec4(0, 0, width, height)), _clearColor(glm::vec4(0, 0, 0, 0))
	{
		colorTexture = GLTexture::NewColorAttachmentTexture2D(width, height);
		glGenFramebuffers(1, &framebufferID);
		bind();
		addColorAttachment();
		unbind();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &framebufferID);
		glDeleteRenderbuffers(1, &renderbuffersID);
		delete colorTexture;
	}

	void GLFrameBuffer::setClearBufferFlags(const ks::ClearBufferFlags & flags)
	{
		clearBufferFlags = flags;
	}

	void GLFrameBuffer::setClearColor(const glm::vec4 & color)
	{
		_clearColor = color;
	}

	void GLFrameBuffer::setViewport(const int x, const int y, const int width, const int height)
	{
		_viewport = glm::ivec4(x, y, width, height);
	}

	void GLFrameBuffer::addColorAttachment()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetRendererID(), 0);
		glGenRenderbuffers(1, &renderbuffersID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffersID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffersID);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void GLFrameBuffer::clear() const
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

	void GLFrameBuffer::clearColor() const
	{
		glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	}

	void GLFrameBuffer::viewport() const
	{
		glViewport(_viewport.x, _viewport.y, _viewport.z, _viewport.w);
	}

	void GLFrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		viewport();
		clearColor();
		clear();
	}

	void GLFrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ITexture * GLFrameBuffer::getTexture() const
	{
		return colorTexture;
	}
}