#include <OpenGL/GLTexture.hpp>
#include <iostream>
#include <assert.h>
#include "OpenGL/GLUtil.hpp"

namespace ks
{
	GLTexture::GLTexture(const GLTextureImageTargetType TextureImageTargetType,
		const int Width,
		const int Height,
		const GLInternalFormatType InternalFormatType,
		const GLPixelFormatType PixelFormatType,
		const int Level,
		const GLDataType DataType,
		bool IsGenerateMipmap,
		const std::vector<const void*> Buffers)
		:Width(Width), Height(Height), TextureImageTargetType(TextureImageTargetType), RendererID(0)
	{
		assert(Level >= 0);
		if (GL_TEXTURE_RECTANGLE == static_cast<int>(TextureImageTargetType) || GL_PROXY_TEXTURE_RECTANGLE == static_cast<int>(TextureImageTargetType))
		{
			assert(Level == 0);
		}
		if (GL_TEXTURE_CUBE_MAP == static_cast<int>(TextureImageTargetType))
		{
			assert(Buffers.size() == 6);
		}
		else
		{
			assert(Buffers.size() == 1 || Buffers.size() == 0);
		}
		for (const void* Buffer : Buffers)
		{
			assert(Buffer);
		}
		glGenTextures(1, &RendererID);
		glBindTexture(static_cast<int>(TextureImageTargetType), RendererID);
		if (GL_TEXTURE_CUBE_MAP == static_cast<int>(TextureImageTargetType))
		{
			for (int i = 0; i < Buffers.size(); i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Level, static_cast<int>(InternalFormatType), Width, Height, 0, static_cast<int>(PixelFormatType), static_cast<int>(DataType), Buffers[i]);
			}
		}
		else
		{
			if (Buffers.size() == 0)
			{
				glTexImage2D(static_cast<int>(TextureImageTargetType), Level, static_cast<int>(InternalFormatType), Width, Height, 0, static_cast<int>(PixelFormatType), static_cast<int>(DataType), NULL);
			}
			else
			{
				glTexImage2D(static_cast<int>(TextureImageTargetType), Level, static_cast<int>(InternalFormatType), Width, Height, 0, static_cast<int>(PixelFormatType), static_cast<int>(DataType), Buffers[0]);
			}
		}
		if (IsGenerateMipmap)
		{
			GenerateMipmap();
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &RendererID);
	}

	GLTexture * GLTexture::NewTexture2D(const int Width, const int Height, const GLPixelFormatType PixelFormatType, const void * Buffer)
	{
		GLTexture* texture = new GLTexture(GLTextureImageTargetType::Texture2D,
			Width,
			Height,
			static_cast<GLInternalFormatType>(PixelFormatType),
			PixelFormatType,
			0,
			GLDataType::UnsignedByte,
			true,
			{ Buffer });
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return texture;
	}

	GLTexture * GLTexture::NewTextureCubeMap(int Width, int Height, const GLPixelFormatType PixelFormatType, const std::vector<const void*> Buffers)
	{
		GLTexture* texture = new GLTexture(GLTextureImageTargetType::TextureCubeMap,
			Width,
			Height,
			static_cast<GLInternalFormatType>(PixelFormatType),
			PixelFormatType,
			0,
			GLDataType::UnsignedByte,
			false,
			Buffers);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return texture;
	}

	GLTexture * GLTexture::NewColorAttachmentTexture2D(const int Width, const int Height)
	{
		GLTexture* texture = new GLTexture(GLTextureImageTargetType::Texture2D,
			Width,
			Height,
			static_cast<GLInternalFormatType>(GLPixelFormatType::RGBA),
			GLPixelFormatType::RGBA,
			0,
			GLDataType::UnsignedByte,
			false,
			{ });
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return texture;
	}

	GLTexture * GLTexture::NewDepthMapTexture2D(const int Width, const int Height)
	{
		GLTexture* texture = new GLTexture(GLTextureImageTargetType::Texture2D,
			Width,
			Height,
			static_cast<GLInternalFormatType>(GLPixelFormatType::DepthComponent),
			GLPixelFormatType::DepthComponent,
			0,
			GLDataType::Float,
			false,
			{ });
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		return texture;
	}

	int GLTexture::GetWidth() const
	{
		return Width;
	}

	int GLTexture::GetHeight() const
	{
		return Height;
	}

	unsigned int GLTexture::GetRendererID() const
	{
		return RendererID;
	}

	GLTextureImageTargetType GLTexture::GetTextureImageTargetType() const
	{
		return TextureImageTargetType;
	}

	void GLTexture::SetMinFilter(const GLTextureMinFilterType Type)
	{
		glTexParameteri(static_cast<int>(TextureImageTargetType), GL_TEXTURE_MIN_FILTER, static_cast<int>(Type));
	}

	void GLTexture::SetMagFilter(const GLTextureMagFilterType Type)
	{
		glTexParameteri(static_cast<int>(TextureImageTargetType), GL_TEXTURE_MAG_FILTER, static_cast<int>(Type));
	}

	void GLTexture::SetWrapS(const GLtextureWrapParameterType Type)
	{
		glTexParameteri(static_cast<int>(TextureImageTargetType), GL_TEXTURE_WRAP_S, static_cast<int>(Type));
	}

	void GLTexture::SetWrapT(const GLtextureWrapParameterType Type)
	{
		glTexParameteri(static_cast<int>(TextureImageTargetType), GL_TEXTURE_WRAP_T, static_cast<int>(Type));
	}

	void GLTexture::GenerateMipmap()
	{
		glGenerateMipmap(static_cast<int>(TextureImageTargetType));
	}

	void GLTexture::bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, RendererID);
	}

	void GLTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::read(ks::PixelBuffer & pixelBuffer)
	{
		std::unordered_map<ks::PixelBuffer::FormatType, bool> dic;
		dic[ks::PixelBuffer::FormatType::rgba8] = true;
		dic[ks::PixelBuffer::FormatType::rgb8] = true;
		assert(dic.end() != dic.find(pixelBuffer.getType()));

		//glBindTexture(GL_TEXTURE_2D, RendererID);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		//	pixelBuffer.getMutableData()[0]);
		//auto errorCode = glGetError();

		{
			int imgByteSize = ks::PixelBuffer::getBytesCount(pixelBuffer.getWidth(), pixelBuffer.getHeight(), pixelBuffer.getType());
			unsigned int downloadPboId;
			glGenBuffers(1, &downloadPboId);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPboId);
			glBufferData(GL_PIXEL_PACK_BUFFER, imgByteSize, 0, GL_STREAM_DRAW);
			glReadPixels(0, 0, pixelBuffer.getWidth(), pixelBuffer.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			GLubyte *bufPtr = static_cast<GLubyte *>(glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, imgByteSize, GL_MAP_READ_BIT));
			if (bufPtr) 
			{
				memcpy(pixelBuffer.getMutableData()[0], bufPtr, imgByteSize);
				glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			}
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			glDeleteBuffers(1, &downloadPboId);
			assert(ks::GLErrorChecker::isNoError());
		}

	}

}