#ifndef KSRENDERENGIFrameBuffer
#define KSRENDERENGIFrameBuffer
#include <glm/glm.hpp>
#include "Common/ClearBufferFlags.hpp"
#include "Common/Deletable.hpp"
#include "Texture2D.hpp"

namespace ks
{
	class IFrameBuffer : public IDeletable
	{
	public:
		virtual ~IFrameBuffer() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		//virtual ITexture* getTexture() const = 0;
		virtual ITexture2D* getColorTexture() const = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
	};

}

#endif // !KSRENDERENGIFrameBuffer