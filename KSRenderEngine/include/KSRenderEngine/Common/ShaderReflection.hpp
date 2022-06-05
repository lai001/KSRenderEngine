#ifndef KSRenderEngine_Common_ShaderReflection_hpp
#define KSRenderEngine_Common_ShaderReflection_hpp

#include <vector>
#include <Foundation/Foundation.hpp>
#include "ShaderConductorHelper.hpp"
#include "Common/Uniform.hpp"
#include "Common/VertexBufferLayout.hpp"
#include "Interface/Shader.hpp"

namespace ks
{
	class ShaderReflection
	{
		typedef ShaderConductor::Reflection::ComponentMask ComponentMask;
		typedef ShaderConductor::Reflection::VariableType::DataType DataType;
		typedef ShaderConductor::Reflection::SignatureParameterDesc SignatureParameterDesc;
		typedef ShaderConductor::Reflection::ConstantBuffer ConstantBuffer;
		typedef ShaderConductor::Reflection::VariableDesc VariableDesc;
		typedef ShaderConductor::Reflection::ResourceDesc ResourceDesc;
		typedef ShaderConductor::Compiler::ResultDesc ResultDesc;
		typedef ShaderConductor::ShaderResourceType ShaderResourceType;

	private:
		static unsigned int getCount(const ComponentMask mask);
		static VertexBufferElement::Semantics getSemantics(const std::string& semantic);
		static VertexBufferElement::Type getElementType(const DataType& dataType);
		static UniformInfo getUniformInfo(const ConstantBuffer& constantBuffer, const VariableDesc& variableDesc);

		std::string shaderCode;
		ks::IShader::Type shaderType;
		std::string entryPoint;

		std::vector<UniformBufferInfo> uniformBuffers;
		std::vector<ShaderTexture2DInfo> texture2DNmaes;
		VertexBufferLayout bufferLayout;

		std::vector<UniformBufferInfo> getUniformBuffers(const ResultDesc& resultDesc);
		std::vector<ShaderTexture2DInfo> getTexture2DNmaes(const ResultDesc& resultDesc);
		VertexBufferLayout getBufferLayout(const ResultDesc& resultDesc);

		//ResultDesc vertexResult;
		//ResultDesc fragmentResult;

	public:
		explicit ShaderReflection(const std::string& shaderCode,
			const ks::IShader::Type shaderType, 
			const std::string& entryPoint = "main");

		std::vector<UniformBufferInfo> getUniformBuffers() const noexcept;
		std::vector<ShaderTexture2DInfo> getTexture2DNmaes() const noexcept;
		VertexBufferLayout getBufferLayout() const noexcept;

		static const std::string Texture2DPrefix;
		static const std::string ConstantBlockPrefix;
	};
}

#endif // !KSRenderEngine_Common_ShaderReflection_hpp