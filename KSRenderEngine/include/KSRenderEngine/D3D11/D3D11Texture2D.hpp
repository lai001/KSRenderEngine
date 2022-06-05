#ifndef KSRenderEngine_D3D11_D3D11Texture2D_hpp
#define KSRenderEngine_D3D11_D3D11Texture2D_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include "Platform/windows.hpp"
#include <Foundation/Foundation.hpp>
#include "Common/Texture2DDescription.hpp"
#include "Interface/Texture2D.hpp"
#include "Interface/FrameBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{
	class D3D11Texture2D: public noncopyable, public ITexture2D
	{
	public:
		~D3D11Texture2D();

		static D3D11Texture2D* create(const Texture2DDescription& texture2DDescription,
			const D3D11RenderEngineInfo& engineInfo);

		static D3D11Texture2D* create(const unsigned int width,
			const unsigned int height,
			const TextureFormat textureFormat,
			const void* data,
			const D3D11RenderEngineInfo& engineInfo);

		static D3D11Texture2D* createRenderTargetTexture2D(const unsigned int width,
			const unsigned int height,
			const TextureFormat textureFormat,
			const D3D11RenderEngineInfo& engineInfo);

		static D3D11Texture2D* createReadBackTexture2D(const unsigned int width,
			const unsigned int height,
			const TextureFormat textureFormat,
			const D3D11RenderEngineInfo& engineInfo);

		void bind(unsigned int slot) const override;

		void unbind() override;

		void read(ks::PixelBuffer & pixelBuffer) override;

		static D3D11_TEXTURE2D_DESC toTexture2DDesc(const Texture2DDescription& texture2DDescription);

		ID3D11Texture2D* getNativeTexture2D() const;

		TextureFormat getTextureFormat() const;
		DXGI_FORMAT getNativeTextureFormat() const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		Texture2DDescription getTexture2DDescription() const;

	protected:
		D3D11Texture2D();

	private:
		Texture2DDescription texture2DDescription;
		ID3D11Texture2D* texture2D = nullptr;
		ID3D11ShaderResourceView* texture2DResourceView = nullptr;
		ID3D11SamplerState* samplerState = nullptr;
		D3D11RenderEngineInfo engineInfo;
	};
}

#endif // !KSRenderEngine_D3D11_D3D11Texture2D_hpp