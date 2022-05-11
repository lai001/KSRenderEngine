#include "KSRenderEngine.hpp"
#include "OpenGL/OpenGL.hpp"

namespace ks
{
	RenderEngine::RenderEngine(const RendererType & rendererType)
		:rendererType(rendererType)
	{
		if (rendererType == RendererType::OpenGL)
		{
			//ks::GLInit();
		}
	}

	IFrameBuffer * RenderEngine::createFrameBuffer(const int width, const int height)
	{
		if (rendererType == RendererType::OpenGL)
		{
			return new GLFrameBuffer(width, height);
		}

		return nullptr;
	}

	IShader * RenderEngine::createShader(const std::string & VertexShaderSource, const std::string & FragmentShaderSource, const std::vector<UniformInfo>& createInfos)
	{
		if (rendererType == RendererType::OpenGL)
		{
			return GLShader::create(VertexShaderSource, FragmentShaderSource, createInfos);
		}
		return nullptr;
	}

	IRenderBuffer * RenderEngine::createRenderBuffer(const void* data, const unsigned int size, const VertexBufferLayout& layout, const unsigned int* Data, const unsigned int Count)
	{
		if (rendererType == RendererType::OpenGL)
		{
			return new GLRenderBuffer(data, size, layout, Data, Count);
		}
		return nullptr;
	}

	void RenderEngine::readTexture(const IFrameBuffer* frameBuffer, ks::PixelBuffer & pixelBuffer)
	{
		if (rendererType == RendererType::OpenGL)
		{
			if (frameBuffer)
			{
				frameBuffer->bind();
				ks::GLTextureReader::read(pixelBuffer);
				frameBuffer->unbind();
			}
			else
			{
				ks::GLTextureReader::read(pixelBuffer);
			}
		}
	}

	void RenderEngine::erase(IFrameBuffer * ptr)
	{
		delete ptr;
	}

	void RenderEngine::erase(IShader * ptr)
	{
		delete ptr;
	}

	void RenderEngine::erase(IRenderBuffer * ptr)
	{
		delete ptr;
	}

	void RenderEngine::enableDebug(const bool flag) noexcept
	{
		if (rendererType == RendererType::OpenGL)
		{
			ks::GLErrorFilter errorFilter;
			errorFilter.errorLevelType = GLErrorFilter::ErrorLevelType::error;
			errorFilter.everityType = GLErrorFilter::SeverityType::high;
			ks::GLErrorChecker::enableDebugMessageCallback(&errorFilter, flag);
		}
	}

}
