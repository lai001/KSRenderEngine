#ifndef KSRENDERENGIFrameBuffer
#define KSRENDERENGIFrameBuffer
#include <glm/glm.hpp>
#include "Common/ClearBufferFlags.hpp"
#include "Common/Deletable.hpp"
#include "Texture.hpp"

namespace ks
{
	class IFrameBuffer : public IDeletable
	{
	public:
		virtual ~IFrameBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual ITexture* getTexture() const = 0;
	};

}

#endif // !KSRENDERENGIFrameBuffer