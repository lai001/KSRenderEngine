#include "Common/ShaderReflection.hpp"

namespace ks
{
	const std::string ShaderReflection::Texture2DPrefix = "SPIRV_Cross_Combined";
	const std::string ShaderReflection::ConstantBlockPrefix = "type_";
}

namespace ks
{
	unsigned int ShaderReflection::getCount(const ComponentMask mask)
	{
		unsigned int cout = 0;
		if (mask == (ComponentMask::X | ComponentMask::Y | ComponentMask::Z | ComponentMask::W))
		{
			cout = 4;
		}
		else if (mask == (ComponentMask::X | ComponentMask::Y | ComponentMask::Z))
		{
			cout = 3;
		}
		else if (mask == (ComponentMask::X | ComponentMask::Y))
		{
			cout = 2;
		}
		else if (mask == (ComponentMask::X))
		{
			cout = 1;
		}
		else
		{
			assert(false);
		}
		return cout;
	}

	VertexBufferElement::Semantics ShaderReflection::getSemantics(const std::string & semantic)
	{
		if (stringop::isContains(semantic, "COLOR"))
		{
			return VertexBufferElement::Semantics::color;
		}
		else if (stringop::isContains(semantic, "TEXCOORD"))
		{
			return VertexBufferElement::Semantics::texcoord;
		}
		else if (stringop::isContains(semantic, "POSITION"))
		{
			return VertexBufferElement::Semantics::position;
		}
		else if (stringop::isContains(semantic, "NORMAL"))
		{
			return VertexBufferElement::Semantics::normal;
		}
		else
		{
			assert(false);
		}
		return VertexBufferElement::Semantics::unknow;
	}

	VertexBufferElement::Type ShaderReflection::getElementType(const DataType & dataType)
	{
		std::unordered_map<DataType, VertexBufferElement::Type> dic;
		dic[DataType::Float] = VertexBufferElement::Type::f32;
		dic[DataType::Int] = VertexBufferElement::Type::int32;
		dic[DataType::Uint] = VertexBufferElement::Type::uint32;
		assert(dic.find(dataType) != dic.end());
		return dic.at(dataType);
	}

	UniformInfo ShaderReflection::getUniformInfo(const ConstantBuffer & constantBuffer, const VariableDesc & variableDesc)
	{
		const std::string uniformName = std::string(constantBuffer.Name()) + "." + std::string(variableDesc.name);

		if (variableDesc.type.Type() == DataType::Float)
		{
			if (variableDesc.size == sizeof(glm::mat4))
			{
				return UniformInfo(uniformName, UniformValue::Type::mat4);
			}
			else if (variableDesc.size == sizeof(glm::mat3))
			{
				return UniformInfo(uniformName, UniformValue::Type::mat3);
			}
			else if (variableDesc.size == sizeof(glm::vec4))
			{
				return UniformInfo(uniformName, UniformValue::Type::vec4);
			}
			else if (variableDesc.size == sizeof(glm::vec3))
			{
				return UniformInfo(uniformName, UniformValue::Type::vec3);
			}
			else if (variableDesc.size == sizeof(glm::vec2))
			{
				return UniformInfo(uniformName, UniformValue::Type::vec2);
			}
			else if (variableDesc.size == sizeof(glm::float32))
			{
				return UniformInfo(uniformName, UniformValue::Type::f32);
			}
			else
			{
				assert(false);
			}
		}
		else if (variableDesc.type.Type() == DataType::Int)
		{
			if (variableDesc.size == sizeof(glm::i32))
			{
				return UniformInfo(uniformName, UniformValue::Type::i32);
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
	}

	std::vector<UniformInfo> ShaderReflection::getVertexUniformInfos(const std::string & shaderCode, const std::string & entryPoint)
	{
		assert(false); // TODO:
		ResultDesc desc = ShaderConductorHelper::VSHLSL2SPIRV(shaderCode, entryPoint);
		desc = ShaderConductorHelper::disassembleSPIRV(reinterpret_cast<const unsigned char *>(desc.target.Data()), desc.target.Size());
		const std::string info = std::string(reinterpret_cast<const char *>(desc.target.Data()), desc.target.Size());

		return std::vector<UniformInfo>();
	}

	std::vector<UniformInfo> ShaderReflection::getFragUniformInfos(const std::string & shaderCode, const std::string & entryPoint)
	{
		ResultDesc result = ShaderConductorHelper::PSHLSL2GLSL(shaderCode, entryPoint);
		if (result.hasError)
		{
			std::string errorInfo = std::string(reinterpret_cast<const char*>(result.errorWarningMsg.Data()));
			assert(result.hasError == false);
		}
		assert(result.reflection.Valid());

		std::vector<UniformInfo> uniformInfos;

		for (unsigned int i = 0; i < result.reflection.NumConstantBuffers(); i++)
		{
			const ConstantBuffer& constantBuffer = *result.reflection.ConstantBufferByIndex(i);

			for (unsigned int numVariablesIdx = 0; numVariablesIdx < constantBuffer.NumVariables(); numVariablesIdx++)
			{
				const VariableDesc& variableDesc = *constantBuffer.VariableByIndex(numVariablesIdx);
				const UniformInfo uniformInfo = getUniformInfo(constantBuffer, variableDesc);
				uniformInfos.push_back(uniformInfo);
			}
		}

		return uniformInfos;
	}

	std::vector<UniformBufferInfo> ShaderReflection::getVertexUniformBuffers(const std::string & shaderCode, const std::string & entryPoint)
	{
		assert(false); // TODO:
		return std::vector<UniformBufferInfo>();
	}

	std::vector<UniformBufferInfo> ShaderReflection::getFragUniformBuffers(const std::string & shaderCode, const std::string & entryPoint)
	{
		ResultDesc result = ShaderConductorHelper::PSHLSL2GLSL(shaderCode, entryPoint);
		if (result.hasError)
		{
			std::string errorInfo = std::string(reinterpret_cast<const char*>(result.errorWarningMsg.Data()));
			assert(result.hasError == false);
		}
		assert(result.reflection.Valid());

		std::string glslCode = std::string(reinterpret_cast<const char*>(result.target.Data()), result.target.Size());

		std::vector<UniformBufferInfo> uniformBuffers;

		for (unsigned int i = 0; i < result.reflection.NumConstantBuffers(); i++)
		{
			const ConstantBuffer& constantBuffer = *result.reflection.ConstantBufferByIndex(i);
			std::vector<UniformInfo> uniformInfos;
			for (unsigned int numVariablesIdx = 0; numVariablesIdx < constantBuffer.NumVariables(); numVariablesIdx++)
			{
				const VariableDesc& variableDesc = *constantBuffer.VariableByIndex(numVariablesIdx);
				const UniformInfo uniformInfo = getUniformInfo(constantBuffer, variableDesc);
				uniformInfos.push_back(uniformInfo);
			}
			const UniformBufferInfo uniformBuffer = UniformBufferInfo(constantBuffer.Name(), uniformInfos);
			uniformBuffers.push_back(uniformBuffer);
		}

		return uniformBuffers;
	}

	std::vector<ShaderTexture2DInfo> ShaderReflection::getVertexTexture2DNmaes(const std::string & shaderCode, const std::string & entryPoint)
	{
		assert(false); // TODO:

		std::vector<ShaderTexture2DInfo> infos;
		return infos;
	}

	std::vector<ShaderTexture2DInfo> ShaderReflection::getFragTexture2DNmaes(const std::string & shaderCode, const std::string & entryPoint)
	{
		ResultDesc result = ShaderConductorHelper::PSHLSL2GLSL(shaderCode, entryPoint);
		if (result.hasError)
		{
			std::string errorInfo = std::string(reinterpret_cast<const char*>(result.errorWarningMsg.Data()));
			assert(result.hasError == false);
		}
		assert(result.reflection.Valid());
		//std::string code = std::string(reinterpret_cast<const char*>(result.target.Data()), result.target.Size());
		std::vector<ShaderTexture2DInfo> infos;

		std::vector<const ResourceDesc*> samplers;
		std::vector<const ResourceDesc*> texture2Ds;

		const unsigned int numResources = result.reflection.NumResources();
		for (unsigned int i = 0; i < numResources; i++)
		{
			const ResourceDesc& desc = *result.reflection.ResourceByIndex(i);
			if (desc.type == ShaderResourceType::Sampler)
			{
				samplers.push_back(&desc);
			}
			else if (desc.type == ShaderResourceType::Texture)
			{
				texture2Ds.push_back(&desc);
			}
		}

		for (size_t i = 0; i < texture2Ds.size(); i++)
		{
			const ResourceDesc& texture2DDesc = *texture2Ds[i];
			if (stringop::isStartWith(texture2DDesc.name, ShaderReflection::Texture2DPrefix))
			{
				for (size_t j = 0; j < samplers.size(); j++)
				{
					const ResourceDesc& samplerDesc = *samplers[j];
					if (stringop::isEndWith(texture2DDesc.name, samplerDesc.name))
					{
						std::string actualName = texture2DDesc.name;
						actualName = stringop::replaceAllOccurrences(actualName, ShaderReflection::Texture2DPrefix, "");
						actualName = stringop::replaceAllOccurrences(actualName, samplerDesc.name, "");
						const ShaderTexture2DInfo shaderTexture2DInfo = ShaderTexture2DInfo(actualName, samplerDesc.name);
						infos.push_back(shaderTexture2DInfo);
					}
				}
			}
		}
		return infos;
	}

	VertexBufferLayout ShaderReflection::getBufferLayout(const std::string & shaderCode, const std::string & entryPoint)
	{
		VertexBufferLayout layout = VertexBufferLayout();
		ResultDesc result = ShaderConductorHelper::VSHLSL2GLSL(shaderCode, entryPoint);
		
		if (result.hasError)
		{
			std::string errorInfo = std::string(reinterpret_cast<const char*>(result.errorWarningMsg.Data()));
			assert(result.hasError == false);
		}
		assert(result.reflection.Valid());

		for (unsigned int i = 0; i < result.reflection.NumInputParameters(); i++)
		{
			const SignatureParameterDesc& inputParam = *result.reflection.InputParameter(i);
			const VertexBufferElement element = VertexBufferElement(getElementType(inputParam.componentType),
				getCount(inputParam.mask),
				false,
				getSemantics(inputParam.semantic),
				inputParam.semanticIndex);
			layout = layout.element(element);
		}

		return layout;
	}
}
