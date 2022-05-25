#ifndef KSRenderEngine_Interface_Shader_hpp
#define KSRenderEngine_Interface_Shader_hpp

#include <string>
#include "Common/Uniform.hpp"
#include "Common/Deletable.hpp"
#include "Common/VertexBufferLayout.hpp"
#include "Texture2D.hpp"

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

		enum class ShadingLanguageType
		{
			hlsl,
			glsl,
		};

	public:
		virtual ~IShader() = 0 { };
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
		virtual void setTexture2D(const std::string& name, const ITexture2D& texture2D) = 0;
		virtual VertexBufferLayout getVertexBufferLayout() const = 0;
	};
}

#endif // !KSRenderEngine_Interface_Shader_hpp