#ifndef KSRENDERENGIUniform
#define KSRENDERENGIUniform
#include <glm/glm.hpp>
#include <string>
#include "Texture2D.hpp"

namespace ks
{
	struct UniformValue
	{
	public:
		enum class Type
		{
			i32,
			f32,
			vec2,
			vec3,
			vec4,
			mat3,
			mat4
		};

		UniformValue::Type type;
		union
		{
			int32_t i32;
			float f32;
			glm::vec2 vec2;
			glm::vec3 vec3;
			glm::vec4 vec4;
			glm::mat3 mat3;
			glm::mat4 mat4;
		};

		UniformValue(const int32_t v);
		UniformValue(const float v);
		UniformValue(const glm::vec2 v);
		UniformValue(const glm::vec3 v);
		UniformValue(const glm::vec4 v);
		UniformValue(const glm::mat3 v);
		UniformValue(const glm::mat4 v);

		const void* getData() const noexcept;
	};
	
	struct UniformInfo
	{
	public:
		std::string name;
		UniformValue::Type type;
		explicit UniformInfo(const std::string& name, const UniformValue::Type type) :name(name), type(type) { }
	};

	struct ShaderTexture2DInfo
	{
	public:
		std::string name;
		std::string samplerName;
		explicit ShaderTexture2DInfo(const std::string& name, const std::string& samplerName) :name(name), samplerName(samplerName) { }
	};
}

#endif // !KSRENDERENGIUniform