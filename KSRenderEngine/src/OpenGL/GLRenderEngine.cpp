#include "OpenGL/GLRenderEngine.hpp"
#include "OpenGL/OpenGL.hpp"

namespace ks
{
	GLRenderEngine::GLRenderEngine(GLRenderEngineCreateInfo createInfo)
		:createInfo(createInfo)
	{
		//ks::GLInit();
	}

	IFrameBuffer * GLRenderEngine::createFrameBuffer(const int width, const int height)
	{
		return new GLFrameBuffer(width, height);
	}

	void GLRenderEngine::readTexture(const IFrameBuffer * frameBuffer, ks::PixelBuffer & pixelBuffer)
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

	void GLRenderEngine::erase(IFrameBuffer * ptr)
	{
		delete ptr;
	}

	void GLRenderEngine::erase(IShader * ptr)
	{
		delete ptr;
	}

	void GLRenderEngine::erase(IRenderBuffer * ptr)
	{
		delete ptr;
	}

	void GLRenderEngine::enableDebug(const bool flag)
	{
		ks::GLErrorFilter errorFilter;
		errorFilter.errorLevelType = GLErrorFilter::ErrorLevelType::error;
		errorFilter.everityType = GLErrorFilter::SeverityType::high;
		ks::GLErrorChecker::enableDebugMessageCallback(&errorFilter, flag);
	}

	IShader * GLRenderEngine::createShader(const std::string & VertexShaderSource, const std::string & FragmentShaderSource, const std::vector<UniformInfo>& createInfos, const ks::VertexBufferLayout & layout)
	{
		return GLShader::create(VertexShaderSource, FragmentShaderSource, createInfos);
	}

	IRenderBuffer * GLRenderEngine::createRenderBuffer(const void * vertexBuffer, 
		const unsigned int vertexCount,
		const unsigned int vertexStride,
		const VertexBufferLayout & layout,
		const void * indexBufferData, 
		const unsigned int indexCount,
		const IIndexBuffer::IndexDataType type)
	{
		GLRenderBuffer* buffer = new GLRenderBuffer(vertexBuffer, vertexCount, vertexStride, layout,
			indexBufferData, indexCount, type);
		return buffer;
	}

	void GLRenderEngine::erase(IBlendState * ptr)
	{
		delete ptr;
	}

	void GLRenderEngine::erase(IRasterizerState * ptr)
	{
		delete ptr;
	}

	void GLRenderEngine::erase(IDepthStencilState * ptr)
	{
		delete ptr;
	}

	ks::IBlendState * GLRenderEngine::createBlendState(const BlendStateDescription::Addition& addition,
		const BlendStateDescription& blendStateDescription)
	{
		return nullptr;
	}

	ks::IRasterizerState * GLRenderEngine::createRasterizerState(const ks::RasterizerStateDescription &rasterizerStateDescription)
	{
		return nullptr;
	}

	ks::IDepthStencilState * GLRenderEngine::createDepthStencilState(const ks::DepthStencilStateDescription &depthStencilStateDescription)
	{
		return nullptr;
	}

}
