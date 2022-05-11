#ifndef KSRenderEngine_hpp
#define KSRenderEngine_hpp

#include <Foundation/Foundation.hpp>
#include "Interface/Interface.hpp"

namespace ks
{
	class RenderEngine: public boost::noncopyable
	{

	public:
		enum class RendererType
		{
			OpenGL
		};

		RenderEngine(const RendererType& rendererType);

		IFrameBuffer* createFrameBuffer(const int width, const int height);
		IShader* createShader(const std::string& VertexShaderSource, const std::string& FragmentShaderSource, const std::vector<UniformInfo>& createInfos);
		IRenderBuffer* createRenderBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout, const unsigned int* Data, const unsigned int Count);

		void readTexture(const IFrameBuffer* frameBuffer, ks::PixelBuffer& pixelBuffer);

		void erase(IFrameBuffer* ptr);
		void erase(IShader* ptr);
		void erase(IRenderBuffer* ptr);

		void enableDebug(const bool flag) noexcept;

	private:
		RendererType rendererType;

	};

}

#endif // !KSRenderEngine_hpp