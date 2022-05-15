#ifndef KSRenderEngine_Interface_RasterizerState_hpp
#define	KSRenderEngine_Interface_RasterizerState_hpp

namespace ks
{
	class IRasterizerState
	{
	public:
		virtual ~IRasterizerState() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_RasterizerState_hpp