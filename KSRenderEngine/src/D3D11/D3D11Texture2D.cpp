#include "D3D11/D3D11Texture2D.hpp"
#include <assert.h>
#include <functional>
#include <unordered_map>

namespace ks
{
	D3D11Texture2D::D3D11Texture2D()
	{
	}

	D3D11Texture2D::~D3D11Texture2D()
	{
		assert(texture2D);
		texture2D->Release();

		if (texture2DResourceView)
		{
			texture2DResourceView->Release();
		}
		if (samplerState)
		{
			samplerState->Release();
		}
	}

	D3D11Texture2D * D3D11Texture2D::create(const Texture2DDescription& texture2DDescription,
		const D3D11RenderEngineInfo& engineInfo)
	{
		assert(engineInfo.context);
		assert(engineInfo.device);
		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11Texture2D* texture2D = nullptr;

		std::function<void()> cleanBlock = [&]()
		{
			if (texture2D) { texture2D->Release(); }
		};
		defer { cleanBlock(); };
		
		HRESULT status = S_OK;
		D3D11_TEXTURE2D_DESC texture2DDesc = toTexture2DDesc(texture2DDescription);
		status = d3dDevice->CreateTexture2D(&texture2DDesc, nullptr, &texture2D);
		if (status != S_OK) { return nullptr; }

		D3D11Texture2D* d3d11Texture2D = new D3D11Texture2D();
		cleanBlock = []() {};
		d3d11Texture2D->texture2D = texture2D;
		d3d11Texture2D->texture2DDescription = texture2DDescription;
		d3d11Texture2D->engineInfo = engineInfo;

		if (texture2DDescription.bindFlag.isContains(ks::TextureBindFlag::shaderResource))
		{
			D3D11_TEX2D_SRV tex2dSRV;
			tex2dSRV.MipLevels = texture2DDescription.mipLevels;
			tex2dSRV.MostDetailedMip = 0;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = d3d11Texture2D->getNativeTextureFormat();
			shaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D = tex2dSRV;
			status = d3dDevice->CreateShaderResourceView(texture2D, &shaderResourceViewDesc, &d3d11Texture2D->texture2DResourceView);
			assert(SUCCEEDED(status));

			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.MaxAnisotropy = 4;
			samplerDesc.MipLODBias = 1.0;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MinLOD = 0.0;
			samplerDesc.MaxLOD = 1.0;
			status = d3dDevice->CreateSamplerState(&samplerDesc, &d3d11Texture2D->samplerState);
			assert(SUCCEEDED(status));
		}

		return d3d11Texture2D;
	}

	D3D11Texture2D * D3D11Texture2D::create(const unsigned int width,
		const unsigned int height,
		const TextureFormat textureFormat,
		const void* data,
		const D3D11RenderEngineInfo& engineInfo)
	{
		assert(textureFormat == TextureFormat::R8G8B8A8_UNORM);

		Texture2DDescription texture2DDescription;
		texture2DDescription.width = width;
		texture2DDescription.height = height;
		texture2DDescription.bindFlag = TextureBindFlag::shaderResource;
		texture2DDescription.usage = TextureUsage::dynamic;
		texture2DDescription.mipLevels = 1;
		texture2DDescription.miscFlag = ResourceMiscFlag();
		texture2DDescription.cpuAccessFlag = CPUAccessFlag::write;
		texture2DDescription.arraySize = 1;
		texture2DDescription.sampleDescription.count = 1;
		texture2DDescription.sampleDescription.quality = 0;
		texture2DDescription.textureFormat = textureFormat;
		D3D11Texture2D* d3d11Texture2D = D3D11Texture2D::create(texture2DDescription, engineInfo);

		if (d3d11Texture2D)
		{
			assert(engineInfo.context);
			assert(engineInfo.device);
			ID3D11Device *d3dDevice = engineInfo.device;
			ID3D11DeviceContext *context = engineInfo.context;
			HRESULT status = S_OK;
			ID3D11Texture2D* texture2D = d3d11Texture2D->getNativeTexture2D();
			D3D11_MAPPED_SUBRESOURCE mappedSubresource;
			status = context->Map(texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			assert(SUCCEEDED(status));
			for (size_t i = 0; i < height; i++)
			{
				memcpy(reinterpret_cast<char*>(mappedSubresource.pData) + i * mappedSubresource.RowPitch,
					reinterpret_cast<const char*>(data) + i * width * 4,
					width * 4);
			}
			context->Unmap(texture2D, 0);

			return d3d11Texture2D;
		}
		else
		{
			return nullptr;
		}
	}

	D3D11Texture2D * D3D11Texture2D::createRenderTargetTexture2D(const unsigned int width,
		const unsigned int height,
		const TextureFormat textureFormat,
		const D3D11RenderEngineInfo& engineInfo)
	{
		Texture2DDescription texture2DDescription;
		texture2DDescription.width = width;
		texture2DDescription.height = height;
		texture2DDescription.bindFlag = TextureBindFlag::renderTarget | TextureBindFlag::shaderResource;
		texture2DDescription.usage = TextureUsage::default;
		texture2DDescription.mipLevels = 1;
		texture2DDescription.miscFlag = ResourceMiscFlag();
		texture2DDescription.cpuAccessFlag = CPUAccessFlag();
		texture2DDescription.arraySize = 1;
		texture2DDescription.sampleDescription.count = 1;
		texture2DDescription.sampleDescription.quality = 0;
		texture2DDescription.textureFormat = textureFormat;
		D3D11Texture2D* d3d11Texture2D = D3D11Texture2D::create(texture2DDescription, engineInfo);
		return d3d11Texture2D;
	}

	D3D11Texture2D * D3D11Texture2D::createReadBackTexture2D(const unsigned int width,
		const unsigned int height,
		const TextureFormat textureFormat,
		const D3D11RenderEngineInfo& engineInfo)
	{
		Texture2DDescription texture2DDescription;
		texture2DDescription.width = width;
		texture2DDescription.height = height;
		texture2DDescription.bindFlag = TextureBindFlag();
		texture2DDescription.usage = TextureUsage::staging;
		texture2DDescription.mipLevels = 1;
		texture2DDescription.miscFlag = ResourceMiscFlag();
		texture2DDescription.cpuAccessFlag = CPUAccessFlag::read;
		texture2DDescription.arraySize = 1;
		texture2DDescription.sampleDescription.count = 1;
		texture2DDescription.sampleDescription.quality = 0;
		texture2DDescription.textureFormat = textureFormat;
		D3D11Texture2D* d3d11Texture2D = D3D11Texture2D::create(texture2DDescription, engineInfo);
		return d3d11Texture2D;
	}

	void D3D11Texture2D::bind(unsigned int slot) const
	{
		assert(engineInfo.context);
		assert(engineInfo.device);
		assert(texture2D);
		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;

		//D3D11_TEXTURE2D_DESC texture2DDesc;
		//texture2D->GetDesc(&texture2DDesc);
		if (texture2DDescription.bindFlag.isContains(ks::TextureBindFlag::shaderResource))
		{
			assert(texture2DResourceView);
			assert(samplerState);
			d3dDeviceContext->PSSetShaderResources(slot, 1, &texture2DResourceView);
			d3dDeviceContext->PSSetSamplers(slot, 1, &samplerState);
			d3dDeviceContext->VSSetShaderResources(slot, 1, &texture2DResourceView);
			d3dDeviceContext->VSSetSamplers(slot, 1, &samplerState);
		}
		else
		{
			assert(false);
		}
	}

	void D3D11Texture2D::unbind()
	{
	}

	void D3D11Texture2D::read(ks::PixelBuffer & pixelBuffer)
	{
	}

	D3D11_TEXTURE2D_DESC D3D11Texture2D::toTexture2DDesc(const Texture2DDescription & texture2DDescription)
	{
		unsigned int CPUAccessFlags = 0;
		if (texture2DDescription.cpuAccessFlag.isContains(CPUAccessFlag::read))
		{
			CPUAccessFlags = CPUAccessFlags | D3D11_CPU_ACCESS_READ;
		}
		if (texture2DDescription.cpuAccessFlag.isContains(CPUAccessFlag::write))
		{
			CPUAccessFlags = CPUAccessFlags | D3D11_CPU_ACCESS_WRITE;
		}

		unsigned int MiscFlags = 0;
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::bufferAllowRawViews))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::bufferStructured))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::drawindirectArgs))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::gdiCompatible))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::generateMips))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::guarded))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_GUARDED;
		}	
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::hwProtected))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_HW_PROTECTED;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::resourceClamp))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_RESOURCE_CLAMP;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::sharedKeyedmutex))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::tilePool))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_TILE_POOL;
		}
		if (texture2DDescription.miscFlag.isContains(ResourceMiscFlag::tiled))
		{
			MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_TILED;
		}
		// TODO: MiscFlags

		unsigned int BindFlags = 0;
		if (texture2DDescription.bindFlag.isContains(TextureBindFlag::renderTarget))
		{
			BindFlags = BindFlags | D3D11_BIND_RENDER_TARGET;
		}
		if (texture2DDescription.bindFlag.isContains(TextureBindFlag::constantBuffer))
		{
			BindFlags = BindFlags | D3D11_BIND_CONSTANT_BUFFER;
		}
		if (texture2DDescription.bindFlag.isContains(TextureBindFlag::shaderResource))
		{
			BindFlags = BindFlags | D3D11_BIND_SHADER_RESOURCE;
		}
		// TODO: BindFlags

		std::unordered_map<TextureUsage, D3D11_USAGE> textureUsageDic;
		textureUsageDic[TextureUsage::default] = D3D11_USAGE_DEFAULT;
		textureUsageDic[TextureUsage::dynamic] = D3D11_USAGE_DYNAMIC;
		textureUsageDic[TextureUsage::immutable] = D3D11_USAGE_IMMUTABLE;
		textureUsageDic[TextureUsage::staging] = D3D11_USAGE_STAGING;
		assert(textureUsageDic.end() != textureUsageDic.find(texture2DDescription.usage));

		DXGI_SAMPLE_DESC dxgiSampleDesc;
		dxgiSampleDesc.Count = texture2DDescription.sampleDescription.count;
		dxgiSampleDesc.Quality = texture2DDescription.sampleDescription.quality;

		std::unordered_map<TextureFormat, DXGI_FORMAT> textureFormatDic;
		textureFormatDic[TextureFormat::R8G8B8A8_UNORM] = DXGI_FORMAT_R8G8B8A8_UNORM;
		assert(textureFormatDic.end() != textureFormatDic.find(texture2DDescription.textureFormat));

		D3D11_TEXTURE2D_DESC d3D11Texture2DDesc;
		d3D11Texture2DDesc.Width = texture2DDescription.width;
		d3D11Texture2DDesc.Height = texture2DDescription.height;
		d3D11Texture2DDesc.ArraySize = texture2DDescription.arraySize;
		d3D11Texture2DDesc.MiscFlags = MiscFlags;
		d3D11Texture2DDesc.MipLevels = texture2DDescription.mipLevels;
		d3D11Texture2DDesc.CPUAccessFlags = CPUAccessFlags;
		d3D11Texture2DDesc.Usage = textureUsageDic.at(texture2DDescription.usage);
		d3D11Texture2DDesc.SampleDesc = dxgiSampleDesc;
		d3D11Texture2DDesc.BindFlags = BindFlags;
		d3D11Texture2DDesc.Format = textureFormatDic.at(texture2DDescription.textureFormat);
		return d3D11Texture2DDesc;
	}

	ID3D11Texture2D * ks::D3D11Texture2D::getNativeTexture2D() const
	{
		return texture2D;
	}

	TextureFormat ks::D3D11Texture2D::getTextureFormat() const
	{
		return texture2DDescription.textureFormat;
	}

	DXGI_FORMAT ks::D3D11Texture2D::getNativeTextureFormat() const
	{
		D3D11_TEXTURE2D_DESC texture2DDesc;
		texture2D->GetDesc(&texture2DDesc);
		return texture2DDesc.Format;
	}

	unsigned int ks::D3D11Texture2D::getWidth() const
	{
		return texture2DDescription.width;
	}

	unsigned int ks::D3D11Texture2D::getHeight() const
	{
		return texture2DDescription.height;
	}
}