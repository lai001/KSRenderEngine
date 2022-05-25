#ifndef KSRenderEngine_OpenGL_GLUniformBuffer_hpp
#define KSRenderEngine_OpenGL_GLUniformBuffer_hpp

#include <vector>
#include <Foundation/Foundation.hpp>
#include "Interface/ConstantBuffer.hpp"
#include "Interface/Shader.hpp"
#include "Common/PackingRules.hpp"

namespace ks
{
	class GLUniformBuffer : public noncopyable, public IConstantBuffer
	{
	private:
		const IShader& shader;
		unsigned int id;
		std::vector<char> constantBuffer;
		UniformBufferInfo uniformBuffer;
		//std::vector<UniformInfo> uniformInfos;
		//std::string uniformBlockName;
		unsigned int blockIndex;
		const unsigned int bindingPoint;

		size_t caclSize(const std::vector<UniformInfo>& uniformInfos, const PackingRules& packingRules) const;
		size_t getOffset(const std::vector<UniformInfo>& uniformInfos, const PackingRules& packingRules, const UniformInfo& uniformInfo) const;
		size_t getSize(const UniformValue::Type dataType) const;
		PackingRules getPackingRules() const noexcept;

	public:
		GLUniformBuffer(const UniformBufferInfo& uniformBuffer,
			const IShader& shader);
		~GLUniformBuffer();

		void bind() const override;
		void unbind() const override;
		void setConstant(const std::string & name, const UniformValue & value) override;
	};
}

#endif // !KSRenderEngine_OpenGL_GLUniformBuffer_hpp