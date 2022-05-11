#ifndef KSRENDERENGINESHADER_HPP
#define KSRENDERENGINESHADER_HPP

#include <string>
#include "Uniform.hpp"
#include "Texture.hpp"

namespace ks
{
	class IShader
	{
	public:
		enum class Type
		{
			vertex,
			fragment
		};

	public:
		virtual ~IShader() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setUniform(const std::string& name, const UniformValue& value) = 0;

	};
}

#endif // !KSRENDERENGINESHADER_HPP