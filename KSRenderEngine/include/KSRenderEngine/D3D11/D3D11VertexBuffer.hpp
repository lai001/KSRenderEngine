#ifndef KSRenderEngine_D3D11_D3D11VertexBuffer_hpp
#define KSRenderEngine_D3D11_D3D11VertexBuffer_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <d3d11.h>
#include <Foundation/Foundation.hpp>
#include "Interface/VertexBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{

	class D3D11VertexBuffer: public boost::noncopyable, public IVertexBuffer
	{
	public:
		D3D11VertexBuffer(void const * buffer,
			const unsigned int vertexCount,
			const unsigned int vertexStride,
			const D3D11RenderEngineInfo & engineInfo);
		~D3D11VertexBuffer();

		void bind() const override;
		void unbind() const override;

	private:
		D3D11RenderEngineInfo engineInfo;
		ID3D11Buffer* verteBuffer = nullptr;
		unsigned int vertexCount;
		unsigned int vertexStride;

		static const unsigned int DefaultStartSlot;
	};


}


#endif // !KSRenderEngine_D3D11_D3D11VertexBuffer_hpp
