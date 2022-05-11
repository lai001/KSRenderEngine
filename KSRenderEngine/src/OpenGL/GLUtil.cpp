#include "OpenGL/GLUtil.hpp"
#include <unordered_map>
#include <iostream>
#include <assert.h>
#include <glad/glad.h>

namespace ks
{
	GLStateInfo GLErrorChecker::debugState() noexcept
	{
		GLStateInfo info;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &info.vao);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &info.ibo);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &info.vbo);

		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &info.iboSize);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &info.vboSize);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &info.maxVertexAttribs);
		info.vertexAttribStates.resize(info.maxVertexAttribs);

		for (int i = 0; i < info.maxVertexAttribs; ++i)
		{
			int isEnable;
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isEnable);
			info.vertexAttribStates[i].isEnable = isEnable;
			if (isEnable)
			{
				int vboBinding;
				glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboBinding);
				info.vertexAttribStates[i].vboBinding = vboBinding;
			}
		}

		return info;
	}

	std::optional<GLErrorDescription> GLErrorChecker::getError() noexcept
	{
		int code = glGetError();
		if (code == GL_NO_ERROR)
		{
			return std::nullopt;
		}
		GLErrorDescription errorDes;
		errorDes.error = static_cast<GLError>(code);
		std::unordered_map<GLError, std::string> dic;
		dic[GLError::invalidEnum] = "GL_INVALID_ENUM";
		dic[GLError::invalidValue] = "GL_INVALID_VALUE";
		dic[GLError::invalidOperation] = "GL_INVALID_OPERATION";
		dic[GLError::stackOverflow] = "GL_STACK_OVERFLOW";
		dic[GLError::stackUnderflow] = "GL_STACK_UNDERFLOW";
		dic[GLError::outOfMemory] = "GL_OUT_OF_MEMORY";
		dic[GLError::invalidFramebufferOperation] = "GL_INVALID_FRAMEBUFFER_OPERATION";
		assert(dic.find(errorDes.error) != dic.end());
		errorDes.description = dic.at(errorDes.error);
		return errorDes;
	}

	void GLErrorChecker::debugMessageCallback(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char *message,
		const void *userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
		GLErrorDescription2 errorDes;
		errorDes.description = "---------------\n";
		errorDes.description += "Debug message (" + std::to_string(id) + "): " + message + "\n";
		errorDes.sourceType = static_cast<GLErrorFilter::SourceType>(source);
		errorDes.severityType = static_cast<GLErrorFilter::SeverityType>(severity);
		errorDes.errorLevelType = static_cast<GLErrorFilter::ErrorLevelType>(type);

		std::unordered_map<GLErrorFilter::SourceType, std::string> sourceDic;
		std::unordered_map<GLErrorFilter::ErrorLevelType, std::string> typeDic;
		std::unordered_map<GLErrorFilter::SeverityType, std::string> severityDic;

		sourceDic[GLErrorFilter::SourceType::api] = "Source: API";
		sourceDic[GLErrorFilter::SourceType::windowSystem] = "Source: Window System";
		sourceDic[GLErrorFilter::SourceType::shaderCompiler] = "Source: Shader Compiler";
		sourceDic[GLErrorFilter::SourceType::thirdParty] = "Source: Third Party";
		sourceDic[GLErrorFilter::SourceType::application] = "Source: Application";
		sourceDic[GLErrorFilter::SourceType::other] = "Source: Other";

		typeDic[GLErrorFilter::ErrorLevelType::error] = "Type: Error";
		typeDic[GLErrorFilter::ErrorLevelType::deprecatedBehavior] = "Type: Deprecated Behaviour";
		typeDic[GLErrorFilter::ErrorLevelType::undefinedBehavior] = "Type: Undefined Behaviour";
		typeDic[GLErrorFilter::ErrorLevelType::portability] = "Type: Portability";
		typeDic[GLErrorFilter::ErrorLevelType::performance] = "Type: Performance";
		typeDic[GLErrorFilter::ErrorLevelType::marker] = "Type: Marker";
		typeDic[GLErrorFilter::ErrorLevelType::pushGroup] = "Type: Push Group";
		typeDic[GLErrorFilter::ErrorLevelType::popGroup] = "Type: Pop Group";
		typeDic[GLErrorFilter::ErrorLevelType::other] = "Type: Other";

		severityDic[GLErrorFilter::SeverityType::low] = "Severity: low";
		severityDic[GLErrorFilter::SeverityType::medium] = "Severity: medium";
		severityDic[GLErrorFilter::SeverityType::high] = "Severity: high";
		severityDic[GLErrorFilter::SeverityType::notification] = "Severity: notification";

		errorDes.description += sourceDic[errorDes.sourceType] + "\n";
		errorDes.description += typeDic[errorDes.errorLevelType] + "\n";
		errorDes.description += severityDic[errorDes.severityType] + "\n";

		std::cout << errorDes.description << std::endl;
		if (errorDes.severityType == GLErrorFilter::SeverityType::high || errorDes.errorLevelType == GLErrorFilter::ErrorLevelType::error)
		{
			assert(false);
		}
	}

	void GLErrorChecker::enableDebugMessageCallback(const GLErrorFilter* filter, const bool flag) noexcept
	{
		if (!GLAD_GL_VERSION_4_3)
		{
			assert(false && "Only support OpenGL 4.3 or above.");
		}
		int flags; 
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			if (flag)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(debugMessageCallback, nullptr);
				if (filter)
				{
					glDebugMessageControl(static_cast<GLenum>(filter->sourceType), static_cast<GLenum>(filter->errorLevelType), static_cast<GLenum>(filter->everityType), 0, nullptr, GL_TRUE);
				}
				else
				{
					glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
				}
			}
			else
			{
				glDisable(GL_DEBUG_OUTPUT);
				glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(nullptr, nullptr);
			}
		}
		else
		{
			assert(false && "Call glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);");
		}
	}

	bool GLErrorChecker::isNoError() noexcept
	{
		return ks::GLErrorChecker::getError() == std::nullopt;
	}
}

namespace ks
{
	void GLTextureReader::read(ks::PixelBuffer & pixelBuffer) noexcept
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