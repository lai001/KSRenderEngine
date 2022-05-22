#ifndef KSRenderEngine_Common_Deletable_hpp
#define KSRenderEngine_Common_Deletable_hpp

namespace ks
{
	class IDeletable
	{
	public:
		virtual ~IDeletable() = 0 { };
	};
}

#endif // !KSRenderEngine_Common_Deletable_hpp