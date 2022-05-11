#ifndef KSRENDERENGIVertexBufferLayout
#define KSRENDERENGIVertexBufferLayout

#include <vector>
#include "VertexBufferElement.hpp"

namespace ks
{
	struct VertexBufferLayout
	{
	public:
		VertexBufferLayout& f32(const unsigned int count, bool isNormalized = false);
		VertexBufferLayout& uint32(const unsigned int count, bool isNormalized = false);
		VertexBufferLayout& ubyte(const unsigned int count, bool isNormalized = false);
		VertexBufferLayout& int32(const unsigned int count, bool isNormalized = false);

		std::vector<VertexBufferElement> getElements() const;
		unsigned int getStride() const;

	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0;
	};
}

#endif // !KSRENDERENGIVertexBufferLayout