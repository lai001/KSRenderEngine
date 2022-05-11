#ifndef KSRENDERENGINEGLFRAMEBUFFERHPP
#define KSRENDERENGINEGLFRAMEBUFFERHPP

#include <Foundation/Foundation.hpp>
#include "Interface/FrameBuffer.hpp"
#include "GLTexture.hpp"
#include "GLShader.hpp"

namespace ks
{

	class GLFrameBuffer: public boost::noncopyable, public ks::IFrameBuffer
	{

	private:
		unsigned int framebufferID;

		unsigned int renderbuffersID;

		int width;

		int height;

		GLTexture* colorTexture;

		glm::vec4 _clearColor;
		glm::ivec4 _viewport;

		ks::ClearBufferFlags clearBufferFlags;

		void addColorAttachment();

		void clear() const;

		void clearColor() const;

		void viewport() const;

	public:
		GLFrameBuffer(const int width, const int height);

		~GLFrameBuffer();

		void setClearBufferFlags(const ks::ClearBufferFlags& flags) override;

		void setClearColor(const glm::vec4& clearColor) override;

		void setViewport(const int x, const int y, const int width, const int height) override;

		void bind() const override;

		void unbind() const override;

		ITexture * getTexture() const override;
	};

}


#endif // !KSRENDERENGINEGLFRAMEBUFFERHPP