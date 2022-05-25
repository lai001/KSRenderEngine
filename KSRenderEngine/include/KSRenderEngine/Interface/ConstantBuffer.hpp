#ifndef KSRenderEngine_Interface_ConstantBuffer_hpp
#define KSRenderEngine_Interface_ConstantBuffer_hpp

#include <string>
#include "Common/Uniform.hpp"

namespace ks
{
	class IConstantBuffer
	{
	public:
		virtual ~IConstantBuffer() = 0 {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setConstant(const std::string& name, const UniformValue& value) = 0;
	};
}

#endif // !KSRenderEngine_Interface_ConstantBuffer_hpp