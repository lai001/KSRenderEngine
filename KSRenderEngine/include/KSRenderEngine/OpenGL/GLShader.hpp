#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <Foundation/Foundation.hpp>
#include <glm/glm.hpp>

#include "Interface/Shader.hpp"
#include "Interface/Uniform.hpp"
#include "Common/RendererError.hpp"
#include "GLTexture.hpp"

namespace ks
{
	class GLShader : public boost::noncopyable, public IShader
	{
	private:
		unsigned int slot;
		unsigned int textureCount;
		std::vector<UniformInfo> uniformCreateInfos;
		std::unordered_map<std::string, int> uniformLocations;

		void findUniformLocations();
		void SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3);
		void SetUniform4fv(const std::string& Name, const glm::vec4& Value, const unsigned int count = 1);
		void SetUniform3f(const std::string& Name, float V0, float V1, float V2);
		void SetUniform3fv(const std::string& Name, const glm::vec3 &Value, const unsigned int count = 1);
		void SetUniform1i(const std::string & Name, int V0);
		void SetUniform1f(const std::string & Name, float V0);
		void SetUniformMat4fv(const std::string & Name, const glm::mat4& Matrix, const unsigned int count = 1);
		static unsigned int compileShader(const IShader::Type shaderType, const std::string & shaderSource, RendererError* error);

	public:
		~GLShader();

		static unsigned int InvalidID;
		unsigned int RendererID = GLShader::InvalidID;

		static GLShader* create(const std::string& VertexShaderSource, const std::string& FragmentShaderSource, const std::vector<UniformInfo> createInfos);

		void setUniform(const std::string& name, const UniformValue& value) override;
		//void setUniform(const std::string& name, const ITexture& texture) override;

		void bind() const override;
		void unbind() const override;
	};
}