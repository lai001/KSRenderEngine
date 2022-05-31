#ifndef KSRenderEngine_Common_ClearBufferFlags_hpp
#define KSRenderEngine_Common_ClearBufferFlags_hpp

#include <Foundation/Foundation.hpp>

namespace ks
{
	struct ClearBufferFlags : public ks::BitFlag<int, ClearBufferFlags>
	{
		explicit ClearBufferFlags(int v = 1 << 0) : ks::BitFlag<int, ClearBufferFlags>(v) { }
		static const ClearBufferFlags none;
		static const ClearBufferFlags depth;
		static const ClearBufferFlags color;
		static const ClearBufferFlags stencil;
	};
}

#endif // !KSRenderEngine_Common_ClearBufferFlags_hpp