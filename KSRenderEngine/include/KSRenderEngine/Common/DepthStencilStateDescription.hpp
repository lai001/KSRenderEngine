#ifndef KSRenderEngine_Common_DepthStencilStateDescription_hpp
#define KSRenderEngine_Common_DepthStencilStateDescription_hpp

#include <Foundation/Foundation.hpp>

namespace ks
{
	enum class DepthStencilComparisonFunc
	{
		never,
		less,
		equal,
		lessEqual,
		greater,
		notEqual,
		greaterEqual,
		always
	};

	enum class DepthStencilOp
	{
		keep,
		zero,
		replace,
		incrSat,
		decrSat,
		invert,
		incr,
		decr
	};

	struct DepthStencilOpDesc
	{
		DepthStencilOp stencilFailOp;
		DepthStencilOp stencilDepthFailOp;
		DepthStencilOp stencilPassOp;
		DepthStencilComparisonFunc stencilFunc;
	};

	struct DepthStencilDepthWriteFlag : public BitFlag<int, DepthStencilDepthWriteFlag>
	{
		explicit DepthStencilDepthWriteFlag(int v = 0) : BitFlag<int, DepthStencilDepthWriteFlag>(v) {};
		static const DepthStencilDepthWriteFlag zero;
		static const DepthStencilDepthWriteFlag all;
	};

	struct StencilReadWriteFlag : public BitFlag<int, StencilReadWriteFlag>
	{
		explicit StencilReadWriteFlag(int v = 0) : BitFlag<int, StencilReadWriteFlag>(v) {};
		static const StencilReadWriteFlag read;
		static const StencilReadWriteFlag write;
	};

	struct DepthStencilStateDescription
	{
		bool isDepthEnable;
		DepthStencilDepthWriteFlag depthWriteFlag;
		DepthStencilComparisonFunc depthFunc;
		bool isStencilEnable;
		StencilReadWriteFlag stencilReadWriteFlag;
		DepthStencilOpDesc frontFace;
		DepthStencilOpDesc backFace;

		static DepthStencilStateDescription getDefault();
	};
}

#endif // !KSRenderEngine_Common_DepthStencilStateDescription_hpp