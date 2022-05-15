#ifndef KSRenderEngine_D3D11_D3D11IndexBuffer_hpp
#define KSRenderEngine_D3D11_D3D11IndexBuffer_hpp

#ifndef _WIN32
#error
#endif // _WIN32

#include <Foundation/Foundation.hpp>
#include <d3d11.h>
#include "Interface/IndexBuffer.hpp"
#include "D3D11/D3D11RenderEngine.hpp"

namespace ks
{

	class D3D11IndexBuffer: public boost::noncopyable, public IIndexBuffer
	{
	public:
		explicit D3D11IndexBuffer(const void* data, 
			unsigned int count,
			const IndexDataType indexDataType,
			const D3D11RenderEngineInfo& engineInfo);
		~D3D11IndexBuffer();
		unsigned int getCount() const;

		void bind() const override;
		void unbind() const override;

		IIndexBuffer::IndexDataType getIndexDataType() noexcept override;

	private:
		ID3D11Buffer * indexBuffer = nullptr;
		IndexDataType indexDataType;
		unsigned int count;
		D3D11RenderEngineInfo engineInfo;

	};

}


#endif // !KSRenderEngine_D3D11_D3D11IndexBuffer_hpp
