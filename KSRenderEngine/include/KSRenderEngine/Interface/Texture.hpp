#ifndef KSRenderEngine_Interface_Texture_hpp
#define KSRenderEngine_Interface_Texture_hpp
#include <Foundation/Foundation.hpp>
#include "Common/Deletable.hpp"

namespace ks
{
	class ITexture : public IDeletable
	{
	public:
		virtual ~ITexture() = 0 { };
		virtual void bind(unsigned int slot) const = 0;
		virtual void unbind() = 0;
		virtual void read(ks::PixelBuffer& pixelBuffer) = 0;
	};
}

#endif // !KSRenderEngine_Interface_Texture_hpp