#pragma once
#include <Foundation/Foundation.hpp>

namespace ks
{
	class GLVertexArray : public boost::noncopyable
	{
	private:
		unsigned int RendererID;
	public:
		GLVertexArray();
		~GLVertexArray();

		int getRendererID();

		void bind() const;
		void unbind();
	};
}