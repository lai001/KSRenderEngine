#ifndef KSRENDERENGIFrameBuffer
#define KSRENDERENGIFrameBuffer
#include <glm/glm.hpp>
#include "Common/ClearBufferFlags.hpp"
#include "Texture.hpp"

namespace ks
{
	class IFrameBuffer
	{
	public:
		virtual ~IFrameBuffer() = 0 { };
		virtual void setClearBufferFlags(const ks::ClearBufferFlags& flags) = 0;
		virtual void setClearColor(const glm::vec4& clearColor) = 0;
		virtual void setViewport(const int x, const int y, const int width, const int height) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual ITexture* getTexture() const = 0;
	};

}

#endif // !KSRENDERENGIFrameBuffer