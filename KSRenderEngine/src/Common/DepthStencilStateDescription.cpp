#include "Common/DepthStencilStateDescription.hpp"

namespace ks
{
	const StencilReadWriteFlag StencilReadWriteFlag::read = StencilReadWriteFlag(1 << 1);
	const StencilReadWriteFlag StencilReadWriteFlag::write = StencilReadWriteFlag(1 << 2);
}

namespace ks
{
	const DepthStencilDepthWriteFlag DepthStencilDepthWriteFlag::zero = DepthStencilDepthWriteFlag(1 << 1);
	const DepthStencilDepthWriteFlag DepthStencilDepthWriteFlag::all = DepthStencilDepthWriteFlag(1 << 2);
}

namespace ks
{
	DepthStencilStateDescription DepthStencilStateDescription::getDefault()
	{
		DepthStencilStateDescription depthStencilStateDescription;
		depthStencilStateDescription.isDepthEnable = true;
		depthStencilStateDescription.isStencilEnable = false;
		depthStencilStateDescription.depthFunc = DepthStencilComparisonFunc::less;
		depthStencilStateDescription.depthWriteFlag = DepthStencilDepthWriteFlag::all;
		depthStencilStateDescription.stencilReadWriteFlag = StencilReadWriteFlag();
		depthStencilStateDescription.frontFace.stencilDepthFailOp = DepthStencilOp::keep;
		depthStencilStateDescription.frontFace.stencilFailOp = DepthStencilOp::keep;
		depthStencilStateDescription.frontFace.stencilPassOp = DepthStencilOp::keep;
		depthStencilStateDescription.frontFace.stencilFunc = DepthStencilComparisonFunc::always;
		depthStencilStateDescription.backFace = depthStencilStateDescription.frontFace;
		return depthStencilStateDescription;
	}
}