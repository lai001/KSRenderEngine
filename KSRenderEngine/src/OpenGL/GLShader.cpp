#include "OpenGL/GLShader.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
//#include "GLFW/glfw3.h"

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
			uniformLocations[uniformCreateInfos.at(i).name] = glGetUniformLocation(RendererID, uniformCreateInfos.at(i).name.c_str());
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

	GLShader* GLShader::create(const std::string& VertexShaderSource, const std::string& FragmentShaderSource, const std::vector<UniformInfo> createInfos)
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
		shader->textureCount = 0;
		for (size_t i = 0; i < createInfos.size(); i++)
		{
			if (createInfos.at(i).type == UniformValue::Type::texture2d)
			{
				shader->textureCount += 1;
			}
		}
		shader->findUniformLocations();
		isSuccess = true;
		return shader;
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
		if (value.type == UniformValue::Type::texture2d)
		{
			assert(value.getTexture2D());
			const ks::ITexture2D& texture = *value.getTexture2D();
			texture.bind(slot);
			SetUniform1i(name, slot);
			slot += 1;
			slot = slot % textureCount;
		}
		else
		{
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

	//void ks::GLShader::setUniform(const std::string& name, const ITexture& texture)
	//{
	//	//const ks::ITexture *texture2D = value.getTexture2D();
	//	texture.bind(slot);
	//	SetUniform1i(name, slot);
	//	slot += 1;
	//	slot = slot % textureCount;
	//}

	//void GLShader::SetTexture(const std::string & Name, const GLTexture& Texture)
	//{
	//	unsigned int Slot;
	//	if (TextureSlotCache.find(Name) == TextureSlotCache.end())
	//	{
	//		std::vector<unsigned int> values;
	//		values.reserve(TextureSlotCache.size());

	//		auto maxValue = std::max_element(values.begin(), values.end());
	//		if (maxValue == values.end())
	//		{
	//			Slot = 0;
	//		}
	//		else
	//		{
	//			Slot = *maxValue + 1;
	//		}
	//		TextureSlotCache[Name] = Slot;
	//	}
	//	else
	//	{
	//		Slot = TextureSlotCache[Name];
	//	}

	//	Texture.Bind(Slot);
	//	SetUniform1i(Name, Slot);
	//}

	//GLShader * GLShader::NewWithSource(const std::string & VertexSourceCode, const std::string & FragSourceCode)
	//{
	//	try
	//	{
	//		int ID = GLShader::CreateShader(VertexSourceCode, FragSourceCode);
	//		GLShader* Shader = new GLShader();
	//		Shader->RendererID = ID;
	//		return Shader;
	//	}
	//	catch (const std::exception E)
	//	{
	//		return nullptr;
	//	}
	//}

	//GLShader * GLShader::Cache(const std::string & Name)
	//{
	//	GLShader* Shader = GLShader::Shaders[Name];
	//	return Shader;
	//}

	//GLShader * GLShader::NewOrCache(const std::string & VertexSourceCode, const std::string & FragSourceCode, const std::string & Name)
	//{
	//	GLShader* Shader = Cache(Name);
	//	if (Shader == nullptr)
	//	{
	//		Shader = NewWithSource(VertexSourceCode, FragSourceCode);
	//		if (Shader)
	//		{
	//			GLShader::Shaders[Name] = Shader;
	//		}
	//	}
	//	return Shader;
	//}


}