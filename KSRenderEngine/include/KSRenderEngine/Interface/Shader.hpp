#ifndef KSRENDERENGINESHADER_HPP
#define KSRENDERENGINESHADER_HPP

#include <string>
#include "Uniform.hpp"
#include "Texture2D.hpp"
#include "Common/Deletable.hpp"

namespace ks
{
	class IShader : public IDeletable
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
		virtual void setTexture2D(const std::string& name, const ITexture2D& texture2D) = 0;
	};
}

#endif // !KSRENDERENGINESHADER_HPP