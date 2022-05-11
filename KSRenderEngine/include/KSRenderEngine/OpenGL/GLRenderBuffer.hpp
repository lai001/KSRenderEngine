#ifndef KSRenderEngineGLRenderBuffer
#define KSRenderEngineGLRenderBuffer

#include "Interface/RenderBuffer.hpp"
#include "GLVertexArray.hpp"
#include "GLIndexBuffer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLShader.hpp"

namespace ks
{
	class GLRenderBuffer : public IRenderBuffer
	{
	private:
		const GLShader* shader = nullptr;
		std::unique_ptr<GLVertexArray> vertexArray = nullptr;
		std::unique_ptr<GLVertexBuffer> vertextBuffer = nullptr;
		std::unique_ptr<GLIndexBuffer> indexBuffer = nullptr;
		const IFrameBuffer* frameBuffer = nullptr;

	public:
		GLRenderBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout, const unsigned int* Data, const unsigned int Count);

		void commit() override;
		void setShader(const IShader & shader) override;
		void setFrameBuffer(const IFrameBuffer & frameBuffer) override;
	};

}

#endif // !KSRenderEngineGLRenderBuffer
