#include "OpenGL/GLUniformBuffer.hpp"
#include <glad/glad.h>
#include "OpenGL/GLShader.hpp"
#include "Common/ShaderReflection.hpp"

namespace ks
{
	GLUniformBuffer::GLUniformBuffer(const UniformBufferInfo& uniformBuffer,
		const IShader& shader)
		:uniformBuffer(uniformBuffer), shader(shader), bindingPoint(0)
	{
		assert(dynamic_cast<const GLShader*>(&shader));
		const GLShader& glShader = dynamic_cast<const GLShader&>(shader);
		const PackingRules packingRules = getPackingRules();
		const size_t size = caclSize(uniformBuffer.uniformInfos, packingRules);
		const unsigned int program = glShader.getID();
		const std::string uniformBlockName = ShaderReflection::ConstantBlockPrefix + uniformBuffer.name;
		blockIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
		assert(blockIndex != GL_INVALID_INDEX);
		glUniformBlockBinding(program, blockIndex, bindingPoint);
		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, id, 0, size);
		constantBuffer = std::vector<char>(size);
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void GLUniformBuffer::setConstant(const std::string & name, const UniformValue & value)
	{
		bool isFind = false;
		for (const UniformInfo& info : uniformBuffer.uniformInfos)
		{
			if (info.name == name)
			{
				const size_t dataTypeSize = getSize(value.type);
				const PackingRules packingRules = getPackingRules();
				const size_t offset = getOffset(uniformBuffer.uniformInfos, packingRules, info);
				memcpy(constantBuffer.data() + offset, value.getData(), dataTypeSize);
				isFind = true;
				break;
			}
		}
		assert(isFind);
	}

	void GLUniformBuffer::bind() const
	{
		const GLShader& glShader = dynamic_cast<const GLShader&>(shader);
		const PackingRules packingRules = getPackingRules();
		const size_t size = caclSize(uniformBuffer.uniformInfos, packingRules);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, constantBuffer.data());
	}

	void GLUniformBuffer::unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	size_t GLUniformBuffer::caclSize(const std::vector<UniformInfo>& uniformInfos, const PackingRules & packingRules) const
	{
		size_t size = 0;
		for (size_t i = 0; i < uniformInfos.size(); i++)
		{
			const UniformValue::Type dataType = uniformInfos[i].type;
			const size_t dataTypeSize = getSize(dataType);
			const size_t forward = size % packingRules.boundary;
			if (forward + dataTypeSize > packingRules.boundary)
			{
				size = alignment(size, packingRules.boundary);
				size = size + dataTypeSize;
			}
			else
			{
				size = size + dataTypeSize;
			}
		}
		size = alignment(size, packingRules.boundary);
		return size;
	}

	size_t GLUniformBuffer::getOffset(const std::vector<UniformInfo>& uniformInfos, const PackingRules & packingRules, const UniformInfo & uniformInfo) const
	{
		size_t size = 0;
		size_t offset;
		for (size_t i = 0; i < uniformInfos.size(); i++)
		{
			const UniformValue::Type dataType = uniformInfos[i].type;
			const size_t dataTypeSize = getSize(dataType);
			const size_t forward = size % packingRules.boundary;
			if (forward + dataTypeSize > packingRules.boundary)
			{
				size = alignment(size, packingRules.boundary);
				size = size + dataTypeSize;
			}
			else
			{
				size = size + dataTypeSize;
			}
			if (uniformInfo.name == uniformInfos[i].name)
			{
				offset = size - dataTypeSize;
				break;
			}
		}
		size = alignment(size, packingRules.boundary);
		return offset;
	}

	size_t GLUniformBuffer::getSize(const UniformValue::Type dataType) const
	{
		std::unordered_map<UniformValue::Type, size_t> dic;
		dic[UniformValue::Type::i32] = 4;
		dic[UniformValue::Type::f32] = 4;
		dic[UniformValue::Type::vec2] = 2 * 4;
		dic[UniformValue::Type::vec3] = 4 * 3;
		dic[UniformValue::Type::vec4] = 4 * 4;
		dic[UniformValue::Type::mat3] = 3 * 3 * 4;
		dic[UniformValue::Type::mat4] = 4 * 4 * 4;
		assert(dic.end() != dic.find(dataType));
		const size_t dataTypeSize = dic.at(dataType);
		return dataTypeSize;
	}

	PackingRules GLUniformBuffer::getPackingRules() const noexcept
	{
		PackingRules packingRules;
		packingRules.pack = 4;
		packingRules.boundary = 16;
		return packingRules;
	}
}