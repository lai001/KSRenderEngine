#ifndef KSRenderEngine_Common_RasterizerStateDescription_hpp
#define KSRenderEngine_Common_RasterizerStateDescription_hpp

namespace ks
{
	enum class RasterizerStateFillMode
	{
		wireframe,
		solid
	};

	enum class RasterizerStateCullMode
	{
		none,
		front,
		back,
	};

	struct RasterizerStateDescription
	{
		RasterizerStateFillMode fillMode;
		RasterizerStateCullMode cullMode;
		bool frontCounterClockwise;
		int depthBias;
		float depthBiasClamp;
		float slopeScaledDepthBias;
		bool depthClipEnable;
		bool scissorEnable;
		bool multisampleEnable;
		bool antialiasedLineEnable;

		static RasterizerStateDescription getDefault();
	};
}

#endif // !KSRenderEngine_Common_RasterizerStateDescription_hpp