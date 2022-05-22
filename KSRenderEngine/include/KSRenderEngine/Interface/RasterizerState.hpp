#ifndef KSRenderEngine_Interface_RasterizerState_hpp
#define	KSRenderEngine_Interface_RasterizerState_hpp
#include "Common/Deletable.hpp"

namespace ks
{
	class IRasterizerState : public IDeletable
	{
	public:
		virtual ~IRasterizerState() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_RasterizerState_hpp