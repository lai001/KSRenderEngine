#include "Common/Texture2DDescription.hpp"

namespace ks
{
	const ResourceMiscFlag ResourceMiscFlag::generateMips = ResourceMiscFlag(1 << 1);
	const ResourceMiscFlag ResourceMiscFlag::shared = ResourceMiscFlag(1 << 2);
	const ResourceMiscFlag ResourceMiscFlag::texturecube = ResourceMiscFlag(1 << 3);
	const ResourceMiscFlag ResourceMiscFlag::drawindirectArgs = ResourceMiscFlag(1 << 4);
	const ResourceMiscFlag ResourceMiscFlag::bufferAllowRawViews = ResourceMiscFlag(1 << 5);
	const ResourceMiscFlag ResourceMiscFlag::bufferStructured = ResourceMiscFlag(1 << 6);
	const ResourceMiscFlag ResourceMiscFlag::resourceClamp = ResourceMiscFlag(1 << 7);
	const ResourceMiscFlag ResourceMiscFlag::sharedKeyedmutex = ResourceMiscFlag(1 << 8);
	const ResourceMiscFlag ResourceMiscFlag::gdiCompatible = ResourceMiscFlag(1 << 9);
	const ResourceMiscFlag ResourceMiscFlag::sharedNthandle = ResourceMiscFlag(1 << 10);
	const ResourceMiscFlag ResourceMiscFlag::restrictedContent = ResourceMiscFlag(1 << 11);
	const ResourceMiscFlag ResourceMiscFlag::restrictSharedResource = ResourceMiscFlag(1 << 12);
	const ResourceMiscFlag ResourceMiscFlag::restrictSharedResourceDriver = ResourceMiscFlag(1 << 13);
	const ResourceMiscFlag ResourceMiscFlag::guarded = ResourceMiscFlag(1 << 14);
	const ResourceMiscFlag ResourceMiscFlag::tilePool = ResourceMiscFlag(1 << 15);
	const ResourceMiscFlag ResourceMiscFlag::tiled = ResourceMiscFlag(1 << 16);
	const ResourceMiscFlag ResourceMiscFlag::hwProtected = ResourceMiscFlag(1 << 17);
	const ResourceMiscFlag ResourceMiscFlag::sharedDisplayable = ResourceMiscFlag(1 << 18);
	const ResourceMiscFlag ResourceMiscFlag::sharedExclusiveWriter = ResourceMiscFlag(1 << 19);
}

namespace ks
{
	const TextureBindFlag TextureBindFlag::vertexBuffer = TextureBindFlag(1 << 1);
	const TextureBindFlag TextureBindFlag::indexBuffer = TextureBindFlag(1 << 2);
	const TextureBindFlag TextureBindFlag::constantBuffer = TextureBindFlag(1 << 3);
	const TextureBindFlag TextureBindFlag::shaderResource = TextureBindFlag(1 << 4);
	const TextureBindFlag TextureBindFlag::streamOutput = TextureBindFlag(1 << 5);
	const TextureBindFlag TextureBindFlag::renderTarget = TextureBindFlag(1 << 6);
	const TextureBindFlag TextureBindFlag::depthStencil = TextureBindFlag(1 << 7);
	const TextureBindFlag TextureBindFlag::unorderedAccess = TextureBindFlag(1 << 8);
	const TextureBindFlag TextureBindFlag::decoder = TextureBindFlag(1 << 9);
	const TextureBindFlag TextureBindFlag::videoEncoder = TextureBindFlag(1 << 10);
}

namespace ks
{
	const CPUAccessFlag CPUAccessFlag::read = CPUAccessFlag(1 << 1);
	const CPUAccessFlag CPUAccessFlag::write = CPUAccessFlag(1 << 2);
}