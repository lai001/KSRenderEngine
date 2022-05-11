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
			mat4,
			texture2d,
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
			const ITexture2D* texture2d = nullptr;
		};

		UniformValue(const int32_t v);
		UniformValue(const float v);
		UniformValue(const glm::vec2 v);
		UniformValue(const glm::vec3 v);
		UniformValue(const glm::vec4 v);
		UniformValue(const glm::mat3 v);
		UniformValue(const glm::mat4 v);
		UniformValue(const ITexture2D* v);

		const void* getData() const noexcept;
		const ITexture2D* getTexture2D() const;
	};
	
	struct UniformInfo
	{
	public:
		std::string name;
		UniformValue::Type type;
		explicit UniformInfo(const std::string& name, const UniformValue::Type type) :name(name), type(type) { }
	};

}

#endif // !KSRENDERENGIUniform