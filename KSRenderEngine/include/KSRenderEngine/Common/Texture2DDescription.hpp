#ifndef KSRenderEngine_Common_Texture2DDescription_hpp
#define KSRenderEngine_Common_Texture2DDescription_hpp

#include <Foundation/Foundation.hpp>

namespace ks
{
	struct SampleDescription
	{
		unsigned int count;
		unsigned int quality;
	};

	struct ResourceMiscFlag : public BitFlag<int, ResourceMiscFlag>
	{
		explicit ResourceMiscFlag(int v = 0) : BitFlag<int, ResourceMiscFlag>(v) { }
		static const ResourceMiscFlag generateMips;
		static const ResourceMiscFlag shared;
		static const ResourceMiscFlag texturecube;
		static const ResourceMiscFlag drawindirectArgs;
		static const ResourceMiscFlag bufferAllowRawViews;
		static const ResourceMiscFlag bufferStructured;
		static const ResourceMiscFlag resourceClamp;
		static const ResourceMiscFlag sharedKeyedmutex;
		static const ResourceMiscFlag gdiCompatible;
		static const ResourceMiscFlag sharedNthandle;
		static const ResourceMiscFlag restrictedContent;
		static const ResourceMiscFlag restrictSharedResource;
		static const ResourceMiscFlag restrictSharedResourceDriver;
		static const ResourceMiscFlag guarded;
		static const ResourceMiscFlag tilePool;
		static const ResourceMiscFlag tiled;
		static const ResourceMiscFlag hwProtected;
		static const ResourceMiscFlag sharedDisplayable;
		static const ResourceMiscFlag sharedExclusiveWriter;
	};

	enum class TextureFormat
	{
		R32G32B32A32_TYPELESS,
		R32G32B32A32_FLOAT,
		R32G32B32A32_UINT,
		R32G32B32A32_SINT,
		R32G32B32_TYPELESS,
		R32G32B32_FLOAT,
		R32G32B32_UINT,
		R32G32B32_SINT,
		R16G16B16A16_TYPELESS,
		R16G16B16A16_FLOAT,
		R16G16B16A16_UNORM,
		R16G16B16A16_UINT,
		R16G16B16A16_SNORM,
		R16G16B16A16_SINT,
		R32G32_TYPELESS,
		R32G32_FLOAT,
		R32G32_UINT,
		R32G32_SINT,
		R32G8X24_TYPELESS,
		D32_FLOAT_S8X24_UINT,
		R32_FLOAT_X8X24_TYPELESS,
		X32_TYPELESS_G8X24_UINT,
		R10G10B10A2_TYPELESS,
		R10G10B10A2_UNORM,
		R10G10B10A2_UINT,
		R11G11B10_FLOAT,
		R8G8B8A8_TYPELESS,
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB,
		R8G8B8A8_UINT,
		R8G8B8A8_SNORM,
		R8G8B8A8_SINT,
		R16G16_TYPELESS,
		R16G16_FLOAT,
		R16G16_UNORM,
		R16G16_UINT,
		R16G16_SNORM,
		R16G16_SINT,
		R32_TYPELESS,
		D32_FLOAT,
		R32_FLOAT,
		R32_UINT,
		R32_SINT,
		R24G8_TYPELESS,
		D24_UNORM_S8_UINT,
		R24_UNORM_X8_TYPELESS,
		X24_TYPELESS_G8_UINT,
		R8G8_TYPELESS,
		R8G8_UNORM,
		R8G8_UINT,
		R8G8_SNORM,
		R8G8_SINT,
		R16_TYPELESS,
		R16_FLOAT,
		D16_UNORM,
		R16_UNORM,
		R16_UINT,
		R16_SNORM,
		R16_SINT,
		R8_TYPELESS,
		R8_UNORM,
		R8_UINT,
		R8_SNORM,
		R8_SINT,
		A8_UNORM,
		R1_UNORM,
		R9G9B9E5_SHAREDEXP,
		R8G8_B8G8_UNORM,
		G8R8_G8B8_UNORM,
		BC1_TYPELESS,
		BC1_UNORM,
		BC1_UNORM_SRGB,
		BC2_TYPELESS,
		BC2_UNORM,
		BC2_UNORM_SRGB,
		BC3_TYPELESS,
		BC3_UNORM,
		BC3_UNORM_SRGB,
		BC4_TYPELESS,
		BC4_UNORM,
		BC4_SNORM,
		BC5_TYPELESS,
		BC5_UNORM,
		BC5_SNORM,
		B5G6R5_UNORM,
		B5G5R5A1_UNORM,
		B8G8R8A8_UNORM,
		B8G8R8X8_UNORM,
		R10G10B10_XR_BIAS_A2_UNORM,
		B8G8R8A8_TYPELESS,
		B8G8R8A8_UNORM_SRGB,
		B8G8R8X8_TYPELESS,
		B8G8R8X8_UNORM_SRGB,
		BC6H_TYPELESS,
		BC6H_UF16,
		BC6H_SF16,
		BC7_TYPELESS,
		BC7_UNORM,
		BC7_UNORM_SRGB,
		AYUV,
		Y410,
		Y416,
		NV12,
		P010,
		P016,
		OPAQUE420,
		YUY2,
		Y210,
		Y216,
		NV11,
		AI44,
		IA44,
		P8,
		A8P8,
		B4G4R4A4_UNORM,
		P208,
		V208,
		V408,
		FORCE_UINT
	};

	enum class TextureUsage
	{
		default,
		immutable,
		dynamic,
		staging
	};

	struct TextureBindFlag : public BitFlag<int, TextureBindFlag>
	{
		explicit TextureBindFlag(int v = 0) : BitFlag<int, TextureBindFlag>(v) { }
		static const TextureBindFlag vertexBuffer;
		static const TextureBindFlag indexBuffer;
		static const TextureBindFlag constantBuffer;
		static const TextureBindFlag shaderResource;
		static const TextureBindFlag streamOutput;
		static const TextureBindFlag renderTarget;
		static const TextureBindFlag depthStencil;
		static const TextureBindFlag unorderedAccess;
		static const TextureBindFlag decoder;
		static const TextureBindFlag videoEncoder;
	};

	struct CPUAccessFlag : public BitFlag<int, CPUAccessFlag>
	{
		explicit CPUAccessFlag(int v = 0) : BitFlag<int, CPUAccessFlag>(v) { }
		static const CPUAccessFlag read;
		static const CPUAccessFlag write;
	};

	enum class TextureMinFilterType
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	enum class TextureMagFilterType
	{
		Nearest,
		Linear
	};

	enum class TextureWrapParameterType
	{
		ClampToEdge,
		ClampToBorder,
		MirroredRepeat,
		Repeat
	};

	struct Texture2DDescription
	{
		unsigned int width;
		unsigned int height;
		unsigned int mipLevels;
		unsigned int arraySize;
		TextureFormat textureFormat;
		SampleDescription sampleDescription;
		TextureUsage usage;
		TextureBindFlag bindFlag;
		CPUAccessFlag cpuAccessFlag;
		ResourceMiscFlag miscFlag = ResourceMiscFlag();

		TextureMinFilterType minFilterType;
		TextureMagFilterType magFilterType;
		TextureWrapParameterType wrapParameterType;
	};
}

#endif // !KSRenderEngine_Common_Texture2DDescription_hpp