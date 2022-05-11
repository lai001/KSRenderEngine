#pragma once
#include <Foundation/Foundation.hpp>
#include "Interface/VertexBuffer.hpp"
#include "Interface/VertexBufferLayout.hpp"

namespace ks
{
	class GLVertexBuffer : public boost::noncopyable, public IVertexBuffer
	{
	private:
		unsigned int RendererID;
		const VertexBufferLayout layout;

	public:
		GLVertexBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout);
		~GLVertexBuffer();

		void bind() const override;
		void unbind() const override;

		VertexBufferLayout getLayout() const;

		void applyLayout() const;

		unsigned int getRendererID() const;
	};
}
