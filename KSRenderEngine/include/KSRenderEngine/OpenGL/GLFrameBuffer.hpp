#ifndef KSRENDERENGINEGLFRAMEBUFFERHPP
#define KSRENDERENGINEGLFRAMEBUFFERHPP

#include <Foundation/Foundation.hpp>
#include "Interface/FrameBuffer.hpp"
#include "GLTexture.hpp"
#include "GLShader.hpp"

namespace ks
{
	class GLFrameBuffer: public noncopyable, public IFrameBuffer
	{
	private:
		unsigned int framebufferID;

		unsigned int renderbuffersID;

		int width;

		int height;

		GLTexture* colorTexture;

		void addColorAttachment();

	public:
		GLFrameBuffer(const int width, const int height);

		~GLFrameBuffer();

		void bind() const override;

		void unbind() const override;

		ITexture2D * getColorTexture() const override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
	};
}

#endif // !KSRENDERENGINEGLFRAMEBUFFERHPP