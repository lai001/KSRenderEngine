#ifndef KSRenderEngine_Interface_BlendState_hpp
#define	KSRenderEngine_Interface_BlendState_hpp

#include "Common/Deletable.hpp"

namespace ks
{
	class IBlendState: public IDeletable
	{
	public:
		virtual ~IBlendState() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_BlendState_hpp