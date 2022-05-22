#ifndef KSRenderEngine_Common_ShaderConductorHelper_hpp
#define KSRenderEngine_Common_ShaderConductorHelper_hpp

#include <string>
#include <Foundation/Foundation.hpp>
#include <ShaderConductor/ShaderConductor.hpp>

namespace ks
{
	class ShaderConductorHelper
	{
	public:
		static ShaderConductor::Compiler::ResultDesc PSHLSL2SPIRV(const std::string& source, const std::string& entryPoint = "main");
		static ShaderConductor::Compiler::ResultDesc PSHLSL2GLSL(const std::string& source, const std::string& entryPoint = "main");

		static ShaderConductor::Compiler::ResultDesc VSHLSL2SPIRV(const std::string& source, const std::string& entryPoint = "main");
		static ShaderConductor::Compiler::ResultDesc VSHLSL2GLSL(const std::string& source, const std::string& entryPoint = "main");

		static ShaderConductor::Compiler::ResultDesc disassembleSPIRV(const unsigned char* source, const unsigned int size);

	private:
		static ShaderConductor::Compiler::ResultDesc convert(const std::string& source,
			const ShaderConductor::ShaderStage stage,
			const ShaderConductor::ShadingLanguage language,
			const std::string& entryPoint = "main");
	};
}

#endif // !KSRenderEngine_Common_ShaderConductorHelper_hpp