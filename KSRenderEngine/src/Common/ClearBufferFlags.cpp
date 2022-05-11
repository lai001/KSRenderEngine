#include "Common/ClearBufferFlags.hpp"

namespace ks
{
	const ClearBufferFlags ClearBufferFlags::none = ClearBufferFlags(1 << 0);
	const ClearBufferFlags ClearBufferFlags::depth = ClearBufferFlags(1 << 1);
	const ClearBufferFlags ClearBufferFlags::color = ClearBufferFlags(1 << 2);
	const ClearBufferFlags ClearBufferFlags::stencil = ClearBufferFlags(1 << 3);
}