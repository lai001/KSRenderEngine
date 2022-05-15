#include "Common/BlendStateDescription.hpp"

namespace ks
{
	 const ColorWriteEnableFlag ColorWriteEnableFlag::red = ColorWriteEnableFlag(1 << 1);
	 const ColorWriteEnableFlag ColorWriteEnableFlag::green = ColorWriteEnableFlag(1 << 2);
	 const ColorWriteEnableFlag ColorWriteEnableFlag::blue = ColorWriteEnableFlag(1 << 3);
	 const ColorWriteEnableFlag ColorWriteEnableFlag::alpha = ColorWriteEnableFlag(1 << 4);

	 ColorWriteEnableFlag ColorWriteEnableFlag::all()
	 {
		 return red | green | blue | alpha;
	 }
}

namespace ks
{
	BlendStateDescription BlendStateDescription::getDefault()
	{
		BlendStateDescription blendStateDescription;
		blendStateDescription.renderTargetWriteFlag = ColorWriteEnableFlag::all();
		blendStateDescription.isBlendEnable = true;
		blendStateDescription.srcBlend = BlendType::srcAlpha;
		blendStateDescription.destBlend = BlendType::invSrcAlpha;
		blendStateDescription.blendOp = BlendOpType::add;
		blendStateDescription.srcBlendAlpha = BlendType::one;
		blendStateDescription.destBlendAlpha = BlendType::invSrcAlpha;
		blendStateDescription.blendOpAlpha = BlendOpType::add;
		return blendStateDescription;
	}

	BlendStateDescription::Addition BlendStateDescription::Addition::getDefault()
	{
		BlendStateDescription::Addition addition;
		addition.isAlphaToCoverageEnable = false;
		addition.isIndependentBlendEnable = false;
		return addition;
	}
}
