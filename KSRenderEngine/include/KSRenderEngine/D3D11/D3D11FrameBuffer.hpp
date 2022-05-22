#ifndef KSRenderEngine_D3D11_D3D11FrameBuffer_hpp
#define KSRenderEngine_D3D11_D3D11FrameBuffer_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include <Foundation/Foundation.hpp>
#include "Interface/FrameBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"
#include "D3D11/D3D11Texture2D.hpp"
#include "D3D11/D3D11RenderTargetView.hpp"

namespace ks
{
	class D3D11FrameBuffer: public noncopyable, public IFrameBuffer
	{
	public:
		D3D11FrameBuffer(const unsigned int width,
			const unsigned int height, 
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11FrameBuffer();


		void bind() const override;
		void unbind() const override;
		ITexture * getTexture() const override;

		ID3D11RenderTargetView* getNativeRenderTargetView() const;
		D3D11RenderTargetView* getRenderTargetView() const;

		void read(PixelBuffer& pixelBuffer) const;

	private:
		D3D11Texture2D* colorTexture2D = nullptr;
		D3D11RenderTargetView* renderTargetView = nullptr;
		D3D11RenderEngineInfo engineInfo;
	};
}

#endif // ! KSRenderEngine_D3D11_D3D11FrameBuffer_hpp