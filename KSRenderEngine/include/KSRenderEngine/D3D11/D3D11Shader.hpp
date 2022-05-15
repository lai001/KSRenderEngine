#ifndef KSRenderEngine_D3D11_D3D11Shader_hpp
#define KSRenderEngine_D3D11_D3D11Shader_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include "Interface/Shader.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{
	class D3D11Shader: public boost::noncopyable, public ks::IShader
	{
	private:
		D3D11RenderEngineInfo engineInfo;
		ID3DBlob* vertexShaderBlob = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;
		ID3DBlob* pixelShaderBlob = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr;

	public:
		~D3D11Shader();
		static D3D11Shader* create(const std::string& vertexShaderSource, 
			const std::string& fragmentShaderSource, 
			const std::vector<UniformInfo> createInfos,
			const ks::VertexBufferLayout& layout,
			const D3D11RenderEngineInfo& engineInfo);

		void bind() const override;
		void unbind() const override;
		void setUniform(const std::string & name, const UniformValue & value) override;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> getInputElementDescs(const ks::VertexBufferLayout& layout);
	};
}

#endif // !KSRenderEngine_D3D11_D3D11Shader_hpp