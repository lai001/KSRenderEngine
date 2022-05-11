#ifndef KSRENDERENGITexture
#define KSRENDERENGITexture
#include <Foundation/Foundation.hpp>

namespace ks
{
	class ITexture
	{
	public:
		virtual ~ITexture() = 0 { };
		virtual void bind(unsigned int slot) const = 0;
		virtual void unbind() = 0;
		virtual void read(ks::PixelBuffer& pixelBuffer) = 0;
	};
}

#endif // !KSRENDERENGITexture