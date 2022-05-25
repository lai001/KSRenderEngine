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

	void GLRenderEngine::enableDebug(const bool flag)
	{
		ks::GLErrorFilter errorFilter;
		errorFilter.errorLevelType = GLErrorFilter::ErrorLevelType::error;
		errorFilter.everityType = GLErrorFilter::SeverityType::high;
		ks::GLErrorChecker::enableDebugMessageCallback(&errorFilter, flag);
	}

	IShader * GLRenderEngine::createShader(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource,
		const std::vector<UniformBufferInfo>& uniformBuffers,
		const std::vector<ShaderTexture2DInfo> texture2DInfos,
		const ks::VertexBufferLayout& layout)
	{
		return GLShader::create(vertexShaderSource,
			fragmentShaderSource,
			uniformBuffers,
			texture2DInfos);
	}

	IShader * GLRenderEngine::createShader(const std::string & VertexShaderSource, const std::string & FragmentShaderSource)
	{
		return GLShader::create(VertexShaderSource, FragmentShaderSource);
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

	ITexture2D * GLRenderEngine::createTexture2D(const unsigned int width,
		const unsigned int height,
		const TextureFormat textureFormat,
		const unsigned char * data)
	{
		assert(data);
		const ks::GLPixelFormatType type = ks::GLPixelFormatType::RGBA; // TODO: More Format
		return GLTexture::NewTexture2D(width, height, type, data);
	}

	void GLRenderEngine::erase(IDeletable * deletable)
	{
		delete deletable;
	}
}