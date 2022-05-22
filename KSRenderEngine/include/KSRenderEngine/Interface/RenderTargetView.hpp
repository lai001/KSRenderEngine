#ifndef KSRenderEngine_Interface_RenderTargetView_hpp
#define KSRenderEngine_Interface_RenderTargetView_hpp
#include "Common/Deletable.hpp"

namespace ks
{
	class IRenderTargetView : public IDeletable
	{
	public:
		virtual ~IRenderTargetView() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_RenderTargetView_hpp