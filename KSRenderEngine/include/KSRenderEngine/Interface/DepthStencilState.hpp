#ifndef KSRenderEngine_Interface_DepthStencilState_hpp
#define	KSRenderEngine_Interface_DepthStencilState_hpp
#include "Common/Deletable.hpp"

namespace ks
{
	class IDepthStencilState : public IDeletable
	{
	public:
		virtual ~IDepthStencilState() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_DepthStencilState_hpp