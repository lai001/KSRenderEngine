#pragma once
#include <Foundation/Foundation.hpp>
#include "Interface/IndexBuffer.hpp"

namespace ks
{
	class GLIndexBuffer : public boost::noncopyable, public IIndexBuffer
	{
	private:
		unsigned int RendererID;
		unsigned int Count;

	public:
		GLIndexBuffer(const unsigned int* Data, unsigned int Count);
		~GLIndexBuffer();

		unsigned int getCount() const;

		void bind() const override;
		void unbind() const override;
	};
}

