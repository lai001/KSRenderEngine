#include "D3D11/D3D11FrameBuffer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace ks
{
	D3D11FrameBuffer::D3D11FrameBuffer(const unsigned int width, 
		const unsigned int height, 
		const D3D11RenderEngineInfo & engineInfo)
		:engineInfo(engineInfo)
	{
		colorTexture2D = D3D11Texture2D::createRenderTargetTexture2D(width, height, TextureFormat::R8G8B8A8_UNORM, engineInfo);
		assert(colorTexture2D);
		renderTargetView = new D3D11RenderTargetView(*colorTexture2D, engineInfo);
		assert(renderTargetView);
	}

	D3D11FrameBuffer::~D3D11FrameBuffer()
	{
		assert(colorTexture2D);
		assert(renderTargetView);
		delete colorTexture2D;
		delete renderTargetView;
	}

	void D3D11FrameBuffer::bind() const
	{
		assert(engineInfo.context);
		assert(engineInfo.device);
		renderTargetView->bind();
	}

	void D3D11FrameBuffer::unbind() const
	{
		renderTargetView->unbind();
	}

	ITexture2D * D3D11FrameBuffer::getColorTexture() const
	{
		return colorTexture2D;
	}

	ID3D11RenderTargetView * D3D11FrameBuffer::getNativeRenderTargetView() const
	{
		return renderTargetView->getNativeView();
	}

	D3D11RenderTargetView * D3D11FrameBuffer::getRenderTargetView() const
	{
		return renderTargetView;
	}

	void D3D11FrameBuffer::read(PixelBuffer & pixelBuffer) const
	{
		std::unordered_map<ks::PixelBuffer::FormatType, bool> dic;
		dic[ks::PixelBuffer::FormatType::rgba8] = true;
		assert(dic.end() != dic.find(pixelBuffer.getType()));

		assert(pixelBuffer.getWidth() == colorTexture2D->getWidth());
		assert(pixelBuffer.getHeight() == colorTexture2D->getHeight());
		assert(engineInfo.context);
		assert(engineInfo.device);

		ID3D11Device *device = engineInfo.device;
		ID3D11DeviceContext *context = engineInfo.context;
		std::unique_ptr<D3D11Texture2D> readTexture2D = std::unique_ptr<D3D11Texture2D>(D3D11Texture2D::createReadBackTexture2D(colorTexture2D->getWidth(),
			colorTexture2D->getHeight(),
			TextureFormat::R8G8B8A8_UNORM,
			engineInfo));
		assert(readTexture2D);
		context->CopyResource(readTexture2D->getNativeTexture2D(), colorTexture2D->getNativeTexture2D());
		unsigned int subresource= D3D11CalcSubresource(0, 0, 0);
		D3D11_MAP_FLAG mapFlag = static_cast<D3D11_MAP_FLAG>(0);
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		context->Map(readTexture2D->getNativeTexture2D(), subresource, D3D11_MAP_READ, mapFlag, &mappedSubresource);
		defer{ context->Unmap(readTexture2D->getNativeTexture2D(), subresource); };
		unsigned int bytes = PixelBuffer::getBytesCount(pixelBuffer.getWidth(), pixelBuffer.getHeight(), pixelBuffer.getType());

		for (size_t i = 0; i < pixelBuffer.getHeight(); i++)
		{
			unsigned char* destData = reinterpret_cast<unsigned char*>(pixelBuffer.getMutableData()[0]);
			unsigned char* srcData = reinterpret_cast<unsigned char*>(mappedSubresource.pData);
			destData = destData + i * pixelBuffer.getWidth() * 4;
			srcData = srcData + i * mappedSubresource.RowPitch;
			memcpy(destData, srcData, pixelBuffer.getWidth() * 4);
		}
	}

	unsigned int D3D11FrameBuffer::getWidth() const
	{
		return	colorTexture2D->getWidth();
	}

	unsigned int D3D11FrameBuffer::getHeight() const
	{
		return	colorTexture2D->getHeight();
	}

}