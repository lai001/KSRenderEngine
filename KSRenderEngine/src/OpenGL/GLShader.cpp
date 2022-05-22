#include "OpenGL/GLShader.hpp"
#include <assert.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Common/ShaderConductorHelper.hpp"
#include "Common/ShaderReflection.hpp"

namespace ks
{
	unsigned int GLShader::InvalidID = -1;

	GLShader::~GLShader()
	{
		assert(RendererID != GLShader::InvalidID);
		glDeleteProgram(RendererID);
	}

	void GLShader::findUniformLocations()
	{
		assert(RendererID != GLShader::InvalidID);
		uniformLocations.clear();
		for (size_t i = 0; i < uniformCreateInfos.size(); i++)
		{
			/* TODO: 
			glBindBuffer(GL_UNIFORM_BUFFER, ...);
			glBufferData(GL_UNIFORM_BUFFER, ...);
			*/
			const std::string key = uniformCreateInfos.at(i).name;
			const int location = glGetUniformLocation(RendererID, key.c_str());
			uniformLocations[key] = location;
		}
		for (size_t i = 0; i < texture2DInfos.size(); i++)
		{
			const std::string key = load(texture2DInfos.at(i)).name;
			const int location = glGetUniformLocation(RendererID, key.c_str());
			uniformLocations[key] = location;
		}
	}

	void GLShader::bind() const
	{
		assert(RendererID != GLShader::InvalidID);
		glUseProgram(RendererID);
	}

	void GLShader::unbind() const
	{
		glUseProgram(0);
	}

	void GLShader::setTexture2D(const std::string & name, const ITexture2D & texture2D)
	{
		bool isFind = false;
		for (size_t i = 0; i < texture2DInfos.size(); i++)
		{
			if (texture2DInfos[i].name == name)
			{
				const std::string actualName = load(texture2DInfos[i]).name;
				SetUniform1i(actualName, i);
				texture2D.bind(i);
				isFind = true;
				break;
			}
		}
		assert(isFind);
	}

	GLShader* GLShader::create(const std::string& VertexShaderSource,
		const std::string& FragmentShaderSource,
		const std::vector<UniformInfo> createInfos,
		const std::vector<ShaderTexture2DInfo> texture2DInfos)
	{
		unsigned int ProgramID = glCreateProgram();
		unsigned int Vs;
		unsigned int Fs;
		bool isSuccess = false;

		defer
		{
			glDeleteShader(Vs);
			glDeleteShader(Fs);
			if (isSuccess)
			{
				return;
			}

			glDeleteProgram(ProgramID);
		};

		RendererError error = RendererError::unknow;
		Vs = compileShader(IShader::Type::vertex, VertexShaderSource, &error);
		if (error == RendererError::compileShaderFail)
		{
			return nullptr;
		}
		error = RendererError::unknow;
		Fs = compileShader(IShader::Type::fragment, FragmentShaderSource, &error);
		if (error == RendererError::compileShaderFail)
		{
			return nullptr;
		}
		glAttachShader(ProgramID, Vs);
		glAttachShader(ProgramID, Fs);
		glLinkProgram(ProgramID);
		glValidateProgram(ProgramID);

		GLShader* shader = new GLShader();
		shader->RendererID = ProgramID;
		shader->uniformCreateInfos = createInfos;
		shader->texture2DInfos = texture2DInfos;
		shader->findUniformLocations();
		isSuccess = true;
		return shader;
	}

	GLShader * GLShader::create(const std::string & vertexShaderSource, const std::string & fragmentShaderSource)
	{
		ShaderConductor::Compiler::ResultDesc desc = ShaderConductorHelper::VSHLSL2GLSL(vertexShaderSource);
		assert(desc.hasError == false);
		std::string glslVertexShaderSource = std::string(reinterpret_cast<const char*>(desc.target.Data()), desc.target.Size());
		desc = ShaderConductorHelper::PSHLSL2GLSL(fragmentShaderSource);
		assert(desc.hasError == false);
		std::string glslFragmentShaderSource = std::string(reinterpret_cast<const char*>(desc.target.Data()), desc.target.Size());;
		std::vector<UniformInfo> uniformInfos = ShaderReflection::getFragUniformInfos(fragmentShaderSource);
		std::vector<ShaderTexture2DInfo> texture2DInfos = ShaderReflection::getFragTexture2DNmaes(fragmentShaderSource);
		return GLShader::create(glslVertexShaderSource, glslFragmentShaderSource, uniformInfos, texture2DInfos);
	}

	unsigned int GLShader::compileShader(const IShader::Type shaderType, const std::string & shaderSource, RendererError* error)
	{
		unsigned int Shader;
		if (shaderType == IShader::Type::vertex)
		{
			Shader = glCreateShader(GL_VERTEX_SHADER);
		}
		else if (shaderType == IShader::Type::fragment)
		{
			Shader = glCreateShader(GL_FRAGMENT_SHADER);
		}
		else
		{
			assert(false);
		}
		const char* Src = shaderSource.c_str();
		glShaderSource(Shader, 1, &Src, nullptr);
		glCompileShader(Shader);

		int Result;
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Result);

		if (Result == false)
		{
			int Length;
			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &Length);
			char* Message = (char*)alloca(Length * sizeof(char));
			glGetShaderInfoLog(Shader, Length, &Length, Message);
			glDeleteShader(Shader);
			if (error)
			{
				*error = RendererError::compileShaderFail;
			}
		}

		return Shader;
	}

	ShaderTexture2DInfo ks::GLShader::load(const ShaderTexture2DInfo & info)
	{
		return ShaderTexture2DInfo(ShaderReflection::Texture2DPrefix + info.name + info.samplerName, info.samplerName);
	}

	void GLShader::SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3)
	{
		glUniform4f(uniformLocations.at(Name), V0, V1, V2, V3);
	}

	void GLShader::SetUniform4fv(const std::string& Name, const glm::vec4& Value, const unsigned int count)
	{
		glUniform4fv(uniformLocations.at(Name), count, glm::value_ptr(Value));
	}

	void GLShader::SetUniform3f(const std::string & Name, float V0, float V1, float V2)
	{
		glUniform3f(uniformLocations.at(Name), V0, V1, V2);
	}

	void GLShader::SetUniform3fv(const std::string & Name, const glm::vec3 & Value, const unsigned int count)
	{
		glUniform3fv(uniformLocations.at(Name), count, glm::value_ptr(Value));
	}

	void GLShader::SetUniform1i(const std::string & Name, int V0)
	{
		glUniform1i(uniformLocations.at(Name), V0);
	}

	void GLShader::SetUniform1f(const std::string & Name, float V0)
	{
		glUniform1f(uniformLocations.at(Name), V0);
	}

	void GLShader::SetUniformMat4fv(const std::string & Name, const glm::mat4& Matrix, const unsigned int count)
	{
		glUniformMatrix4fv(uniformLocations.at(Name), count, GL_FALSE, glm::value_ptr(Matrix));
	}

	void ks::GLShader::setUniform(const std::string& name, const UniformValue& value)
	{
		assert(uniformLocations.end() != uniformLocations.find(name));
		if (value.type == UniformValue::Type::f32)
		{
			SetUniform1f(name, value.f32);
		}
		else if (value.type == UniformValue::Type::i32)
		{
			SetUniform1i(name, value.i32);
		}
		else if (value.type == UniformValue::Type::vec3)
		{
			SetUniform3fv(name, value.vec3, 1);
		}
		else if (value.type == UniformValue::Type::vec4)
		{
			SetUniform4fv(name, value.vec4, 1);
		}
		else if (value.type == UniformValue::Type::mat4)
		{
			SetUniformMat4fv(name, value.mat4, 1);
		}
		else
		{
			assert(false);
		}
	}
}