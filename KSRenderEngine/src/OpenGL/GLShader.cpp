#include "OpenGL/GLShader.hpp"
#include <assert.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Common/ShaderConductorHelper.hpp"
#include "Common/ShaderReflection.hpp"

namespace ks
{
	GLShader::~GLShader()
	{
		for (ks::GLUniformBuffer* glUniformBuffer : constantBuffers)
		{
			assert(glUniformBuffer);
			delete glUniformBuffer;
		}
		constantBuffers.clear();
		glDeleteProgram(RendererID);
	}

	void GLShader::findUniformTexture2DLocations()
	{
		uniformTexture2DLocations.clear();
		for (size_t i = 0; i < texture2DInfos.size(); i++)
		{
			const std::string key = load(texture2DInfos.at(i)).name;
			const int location = glGetUniformLocation(RendererID, key.c_str());
			uniformTexture2DLocations[key] = location;
		}
	}

	void GLShader::bind() const
	{
		useProgram();
		for (ks::GLUniformBuffer* glUniformBuffer : constantBuffers)
		{
			assert(glUniformBuffer);
			glUniformBuffer->bind();
		}
		for (const auto &args : delayBindTexture2Ds)
		{
			const std::function<void()> bindClosure = args.second;
			bindClosure();
		}
	}

	void GLShader::unbind() const
	{
		for (ks::GLUniformBuffer* glUniformBuffer : constantBuffers)
		{
			assert(glUniformBuffer);
			glUniformBuffer->unbind();
		}
		unuseProgram();
	}

	VertexBufferLayout GLShader::getVertexBufferLayout() const
	{
		return vertexBufferLayout;
	}

	void GLShader::setTexture2D(const std::string & name, const ITexture2D & texture2D)
	{
		bool isFind = false;
		for (size_t i = 0; i < texture2DInfos.size(); i++)
		{
			if (texture2DInfos[i].name == name)
			{
				const std::string actualName = load(texture2DInfos[i]).name;
				delayBindTexture2Ds[actualName] = [&texture2D, actualName, this, i]()
				{
					glUniform1i(uniformTexture2DLocations.at(actualName), i);
					texture2D.bind(i);
				};
				isFind = true;
				break;
			}
		}
		assert(isFind);
	}

	GLShader* GLShader::create(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource,
		const ks::VertexBufferLayout& layout,
		const std::vector<UniformBufferInfo> uniformBuffers,
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
		Vs = compileShader(IShader::Type::vertex, vertexShaderSource, &error);
		if (error == RendererError::compileShaderFail)
		{
			return nullptr;
		}
		error = RendererError::unknow;
		Fs = compileShader(IShader::Type::fragment, fragmentShaderSource, &error);
		if (error == RendererError::compileShaderFail)
		{
			return nullptr;
		}
		glAttachShader(ProgramID, Vs);
		glAttachShader(ProgramID, Fs);
		glLinkProgram(ProgramID);
		glValidateProgram(ProgramID);

		GLShader* shader = new GLShader();
		shader->vertexBufferLayout = layout;
		shader->RendererID = ProgramID;
		shader->uniformBuffers = uniformBuffers;
		shader->texture2DInfos = texture2DInfos;
		shader->findUniformTexture2DLocations();
		shader->initConstantBuffer(uniformBuffers);
		isSuccess = true;
		return shader;
	}

	GLShader * GLShader::create(const std::string & vertexShaderSource, const std::string & fragmentShaderSource)
	{
		ShaderConductor::Compiler::ResultDesc desc = ShaderConductorHelper::VSHLSL2GLSL(vertexShaderSource);
		assert(desc.hasError == false);
		const std::string glslVertexShaderSource = std::string(reinterpret_cast<const char*>(desc.target.Data()), desc.target.Size());
		desc = ShaderConductorHelper::PSHLSL2GLSL(fragmentShaderSource);
		assert(desc.hasError == false);
		const std::string glslFragmentShaderSource = std::string(reinterpret_cast<const char*>(desc.target.Data()), desc.target.Size());
		const ShaderReflection vertexShaderReflection = ShaderReflection(vertexShaderSource, IShader::Type::vertex);
		const ShaderReflection fragmentShaderReflection = ShaderReflection(fragmentShaderSource, IShader::Type::fragment);
		const std::vector<UniformBufferInfo> uniformBuffers = fragmentShaderReflection.getUniformBuffers();
		const std::vector<ShaderTexture2DInfo> texture2DInfos = fragmentShaderReflection.getTexture2DNmaes();
		const VertexBufferLayout vertexBufferLayout = vertexShaderReflection.getBufferLayout();
		return GLShader::create(glslVertexShaderSource, glslFragmentShaderSource, vertexBufferLayout, uniformBuffers, texture2DInfos);
	}

	unsigned int ks::GLShader::getID() const noexcept
	{
		return RendererID;
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

	void ks::GLShader::initConstantBuffer(const std::vector<UniformBufferInfo>& uniformBuffers)
	{
		assert(uniformBuffers.size() == 1); // TODO:
		assert(constantBuffers.empty());
		useProgram();
		defer
		{
			unuseProgram();
		};

		GLUniformBuffer* uniformBuffer = new GLUniformBuffer(uniformBuffers.at(0), *this);
		constantBuffers.push_back(uniformBuffer);
	}

	void ks::GLShader::useProgram() const
	{
		glUseProgram(RendererID);
	}

	void ks::GLShader::unuseProgram() const
	{
		glUseProgram(0);
	}

	void ks::GLShader::setUniform(const std::string& name, const UniformValue& value)
	{
		assert(uniformBuffers.size() == 1); // TODO:
		assert(constantBuffers.size() == 1);
		constantBuffers.at(0)->setConstant(name, value);
	}
}