#ifndef KSRenderEngine_Common_ShaderReflection_hpp
#define KSRenderEngine_Common_ShaderReflection_hpp

#include <vector>
#include <Foundation/Foundation.hpp>
#include "ShaderConductorHelper.hpp"
#include "Interface/Uniform.hpp"
#include "Interface/VertexBufferLayout.hpp"

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

	public:
		static std::vector<UniformInfo> getVertexUniformInfos(const std::string& shaderCode, const std::string& entryPoint = "main");
		static std::vector<UniformInfo> getFragUniformInfos(const std::string& shaderCode, const std::string& entryPoint = "main");
		static std::vector<ShaderTexture2DInfo> getVertexTexture2DNmaes(const std::string& shaderCode, const std::string& entryPoint = "main");
		static std::vector<ShaderTexture2DInfo> getFragTexture2DNmaes(const std::string& shaderCode, const std::string& entryPoint = "main");
		static VertexBufferLayout getBufferLayout(const std::string& shaderCode, const std::string& entryPoint = "main");

		static const std::string Texture2DPrefix;
	};
}

#endif // !KSRenderEngine_Common_ShaderReflection_hpp
