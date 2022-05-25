#ifndef KSRenderEngine_OpenGL_GLShader_hpp
#define KSRenderEngine_OpenGL_GLShader_hpp

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <Foundation/Foundation.hpp>
#include <glm/glm.hpp>

#include "Interface/Shader.hpp"
#include "GLUniformBuffer.hpp"
#include "Common/Uniform.hpp"
#include "Common/RendererError.hpp"
#include "Common/PackingRules.hpp"
#include "GLTexture.hpp"

namespace ks
{
	class GLShader : public noncopyable, public IShader
	{
	private:
		VertexBufferLayout vertexBufferLayout;

		std::vector<GLUniformBuffer*> constantBuffers;
		std::vector<UniformBufferInfo> uniformBuffers;
		std::vector<ShaderTexture2DInfo> texture2DInfos;
		std::unordered_map<std::string, int> uniformTexture2DLocations;

		std::unordered_map<std::string, std::function<void()>> delayBindTexture2Ds;

		void findUniformTexture2DLocations();
		static unsigned int compileShader(const IShader::Type shaderType, const std::string & shaderSource, RendererError* error);

		ShaderTexture2DInfo load(const ShaderTexture2DInfo& info);

		void initConstantBuffer(const std::vector<UniformBufferInfo>& uniformBuffers);

		void useProgram() const;
		void unuseProgram() const;

	public:
		~GLShader();

		unsigned int RendererID;

		static GLShader* create(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource,
			const ks::VertexBufferLayout& layout,
			const std::vector<UniformBufferInfo> uniformBuffers,
			const std::vector<ShaderTexture2DInfo> texture2DInfos);

		static GLShader* create(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource);

		unsigned int getID() const noexcept;

		void setUniform(const std::string& name, const UniformValue& value) override;
		void setTexture2D(const std::string & name, const ITexture2D & texture2D) override;

		void bind() const override;
		void unbind() const override;

		VertexBufferLayout getVertexBufferLayout() const override;
	};
}

#endif // !KSRenderEngine_OpenGL_GLShader_hpp