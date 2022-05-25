#ifndef KSRenderEngine_D3D11_D3D11Shader_hpp
#define KSRenderEngine_D3D11_D3D11Shader_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include "Interface/Shader.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "Common/PackingRules.hpp"

namespace ks
{
	class D3D11Shader: public noncopyable, public IShader
	{
	private:
		VertexBufferLayout vertexBufferLayout;
		D3D11RenderEngineInfo engineInfo;
		ID3DBlob* vertexShaderBlob = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;
		ID3DBlob* pixelShaderBlob = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr;
		ID3D11Buffer* constantBuffer = nullptr;
		std::vector<UniformInfo> uniformInfos;
		std::vector<ShaderTexture2DInfo> texture2DInfos;

		void initConstantBuffer(const std::vector<UniformInfo>& uniformInfos);
		size_t caclSize(const std::vector<UniformInfo>& uniformInfos, const PackingRules& packingRules);
		size_t getOffset(const std::vector<UniformInfo>& uniformInfos, const PackingRules& packingRules, const UniformInfo& uniformInfo);
		size_t getSize(const UniformValue::Type dataType);
		PackingRules getPackingRules() const noexcept;

	public:
		~D3D11Shader();
		static D3D11Shader* create(const std::string& vertexShaderSource, 
			const std::string& fragmentShaderSource, 
			const std::vector<UniformBufferInfo> uniformBuffers,
			const std::vector<ShaderTexture2DInfo> texture2DInfos,
			const ks::VertexBufferLayout& layout,
			const D3D11RenderEngineInfo& engineInfo);

		static D3D11Shader* create(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource,
			const D3D11RenderEngineInfo& engineInfo);

		void bind() const override;
		void unbind() const override;
		void setUniform(const std::string & name, const UniformValue & value) override;
		void setTexture2D(const std::string & name, const ITexture2D & texture2D) override;
		VertexBufferLayout getVertexBufferLayout() const override;
		static std::vector<D3D11_INPUT_ELEMENT_DESC> getInputElementDescs(const ks::VertexBufferLayout& layout);
	};
}

#endif // !KSRenderEngine_D3D11_D3D11Shader_hpp