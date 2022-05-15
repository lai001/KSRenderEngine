#include "Common/RasterizerStateDescription.hpp"

namespace ks
{
	RasterizerStateDescription RasterizerStateDescription::getDefault()
	{
		RasterizerStateDescription rasterizerStateDescription;
		rasterizerStateDescription.cullMode = RasterizerStateCullMode::back;
		rasterizerStateDescription.fillMode = RasterizerStateFillMode::solid;
		rasterizerStateDescription.scissorEnable = true;
		rasterizerStateDescription.depthClipEnable = true;
		rasterizerStateDescription.frontCounterClockwise = false;
		rasterizerStateDescription.depthBias = 0;
		rasterizerStateDescription.depthBiasClamp = 0;
		rasterizerStateDescription.slopeScaledDepthBias = 0;
		rasterizerStateDescription.multisampleEnable = false;
		rasterizerStateDescription.antialiasedLineEnable = false;
		return rasterizerStateDescription;
	}
}
