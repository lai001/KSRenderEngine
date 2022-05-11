#include "Interface/Uniform.hpp"

namespace ks
{
	UniformValue::UniformValue(const int32_t v)
		:type(UniformValue::Type::i32), i32(v) { }

	UniformValue::UniformValue(const float v)
		: type(UniformValue::Type::f32), f32(v) { }

	UniformValue::UniformValue(const glm::vec2 v)
		: type(UniformValue::Type::vec2), vec2(v) { }

	UniformValue::UniformValue(const glm::vec3 v)
		: type(UniformValue::Type::vec3), vec3(v) { }

	UniformValue::UniformValue(const glm::vec4 v)
		: type(UniformValue::Type::vec4), vec4(v) { }

	UniformValue::UniformValue(const glm::mat3 v)
		: type(UniformValue::Type::mat3), mat3(v) { }

	UniformValue::UniformValue(const glm::mat4 v)
		: type(UniformValue::Type::mat4), mat4(v) { }

	UniformValue::UniformValue(const ITexture2D * v)
		: type(UniformValue::Type::texture2d), texture2d(v) { }

	const void * UniformValue::getData() const noexcept
	{
		if (type == UniformValue::Type::texture2d)
		{
			return texture2d;
		}
		return &this->f32;
	}

	const ITexture2D * UniformValue::getTexture2D() const
	{
		assert(type == UniformValue::Type::texture2d);
		return texture2d;
	}
}