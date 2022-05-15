#pragma once
#include <Foundation/Foundation.hpp>
#include "Interface/IndexBuffer.hpp"
#include "GLRenderEngine.hpp"

namespace ks
{
	class GLIndexBuffer : public boost::noncopyable, public IIndexBuffer
	{
	private:
		unsigned int rendererID;
		unsigned int count;
		IndexDataType indexDataType;

	public:
		explicit GLIndexBuffer(const void* data, unsigned int count, const IndexDataType indexDataType);
		~GLIndexBuffer();

		unsigned int getCount() const;

		void bind() const override;
		void unbind() const override;

		IIndexBuffer::IndexDataType getIndexDataType() noexcept override;
	};
}