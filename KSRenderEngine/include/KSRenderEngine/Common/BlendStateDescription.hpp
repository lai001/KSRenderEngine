#ifndef KSRenderEngine_Common_BlendStateDescription_hpp
#define KSRenderEngine_Common_BlendStateDescription_hpp

#include <Foundation/Foundation.hpp>

namespace ks
{
	enum class BlendType
	{
		zero,
		one,
		srcColor,
		invSrcColor,
		srcAlpha,
		invSrcAlpha,
		destAlpha,
		invDestAlpha,
		destColor,
		invDestColor,
		srcAlphaSat,
		blendFactor,
		invBlendFactor,
		src1Color,
		invSrc1Color,
		src1Alpha,
		invSrc1Alpha
	};

	enum class BlendOpType
	{
		add,
		subtract,
		revSubtract,
		min,
		max
	};

	struct ColorWriteEnableFlag: public ks::BitFlag<int, ColorWriteEnableFlag>
	{
		explicit ColorWriteEnableFlag(int v = 0) :ks::BitFlag<int, ColorWriteEnableFlag>(v) {}
		static const ColorWriteEnableFlag red;
		static const ColorWriteEnableFlag green;
		static const ColorWriteEnableFlag blue;
		static const ColorWriteEnableFlag alpha;
		static ColorWriteEnableFlag all();
	};

	struct BlendStateDescription
	{
		struct Addition
		{
			bool isAlphaToCoverageEnable;
			bool isIndependentBlendEnable;

			static BlendStateDescription::Addition getDefault();
		};

		bool isBlendEnable;
		BlendType srcBlend;
		BlendType destBlend;
		BlendOpType blendOp;
		BlendType srcBlendAlpha;
		BlendType destBlendAlpha;
		BlendOpType blendOpAlpha;
		ColorWriteEnableFlag renderTargetWriteFlag;

		float blendFactor[4] = { 0.0, 0.0, 0.0, 0.0};
		unsigned int sampleMask = 0xffffffff;

		static BlendStateDescription getDefault();
	};
}

#endif // !KSRenderEngine_Common_BlendStateDescription_hpp