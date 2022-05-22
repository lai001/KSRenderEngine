#include "Common/ShaderConductorHelper.hpp"
#include <assert.h>

namespace ks
{
	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::PSHLSL2SPIRV(const std::string& source, const std::string& entryPoint)
	{
		return convert(source, ShaderConductor::ShaderStage::PixelShader, ShaderConductor::ShadingLanguage::SpirV, entryPoint);
	}

	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::PSHLSL2GLSL(const std::string& source, const std::string& entryPoint)
	{
		return convert(source, ShaderConductor::ShaderStage::PixelShader, ShaderConductor::ShadingLanguage::Glsl, entryPoint);
	}

	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::VSHLSL2SPIRV(const std::string & source, const std::string & entryPoint)
	{
		return convert(source, ShaderConductor::ShaderStage::VertexShader, ShaderConductor::ShadingLanguage::SpirV, entryPoint);
	}

	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::VSHLSL2GLSL(const std::string & source, const std::string & entryPoint)
	{
		return convert(source, ShaderConductor::ShaderStage::VertexShader, ShaderConductor::ShadingLanguage::Glsl, entryPoint);
	}

	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::disassembleSPIRV(const unsigned char * source, const unsigned int size)
	{
		ShaderConductor::Compiler::DisassembleDesc sourceDesc;
		sourceDesc.binary = source;
		sourceDesc.binarySize = size;
		sourceDesc.language = ShaderConductor::ShadingLanguage::SpirV;
		ShaderConductor::Compiler::ResultDesc resultDesc = ShaderConductor::Compiler::Disassemble(sourceDesc);
		return resultDesc;
	}

	ShaderConductor::Compiler::ResultDesc ShaderConductorHelper::convert(const std::string& source,
		const ShaderConductor::ShaderStage stage,
		const ShaderConductor::ShadingLanguage language,
		const std::string& entryPoint)
	{
		ShaderConductor::Compiler::SourceDesc sourceDesc{};
		ShaderConductor::Compiler::TargetDesc targetDesc{};
		sourceDesc.fileName = nullptr;
		sourceDesc.entryPoint = entryPoint.c_str();
		sourceDesc.source = source.c_str();
		sourceDesc.stage = stage;
		targetDesc.language = language;
		if (language == ShaderConductor::ShadingLanguage::Glsl)
		{
			targetDesc.version = "330 core";
		}
		ShaderConductor::Compiler::Options options = {};
		options.needReflection = true;
		options.disableOptimizations = true;
		options.enableDebugInfo = true;
		const ShaderConductor::Compiler::ResultDesc result = ShaderConductor::Compiler::Compile(sourceDesc, options, targetDesc);
		return result;
	}
}