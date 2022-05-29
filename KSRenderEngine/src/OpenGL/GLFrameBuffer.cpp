#include "OpenGL/GLFrameBuffer.hpp"

namespace ks
{
	GLFrameBuffer::GLFrameBuffer(const int width, const int height)
		:width(width), height(height)
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

	void GLFrameBuffer::addColorAttachment()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetRendererID(), 0);
		glGenRenderbuffers(1, &renderbuffersID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffersID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffersID);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void GLFrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	}

	void GLFrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ITexture2D * GLFrameBuffer::getColorTexture() const
	{
		return colorTexture;
	}

	unsigned int GLFrameBuffer::getWidth() const
	{
		return width;
	}

	unsigned int GLFrameBuffer::getHeight() const
	{
		return height;
	}
}