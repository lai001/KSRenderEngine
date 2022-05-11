#ifndef KSRENDERENGINEOPENGLUTILHPP
#define KSRENDERENGINEOPENGLUTILHPP
#include <string>
#include <vector>
#include <optional>
#include <glad/glad.h>
#include <Foundation/Foundation.hpp>

namespace ks
{
	enum class GLError
	{
		invalidEnum = GL_INVALID_ENUM,
		invalidValue = GL_INVALID_VALUE,
		invalidOperation = GL_INVALID_OPERATION,
		stackOverflow = GL_STACK_OVERFLOW,
		stackUnderflow = GL_STACK_UNDERFLOW,
		outOfMemory = GL_OUT_OF_MEMORY,
		invalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION
	};

	struct GLVertexAttribState
	{
		bool isEnable;
		int vboBinding;
	};

	struct GLStateInfo
	{
		int vao;
		int ibo;
		int vbo;
		int iboSize;
		int vboSize;
		int maxVertexAttribs;
		std::vector<GLVertexAttribState> vertexAttribStates;
	};

	struct GLErrorFilter
	{
		enum class SourceType
		{
			api = GL_DEBUG_SOURCE_API,
			windowSystem = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
			shaderCompiler = GL_DEBUG_SOURCE_SHADER_COMPILER,
			thirdParty = GL_DEBUG_SOURCE_THIRD_PARTY,
			application = GL_DEBUG_SOURCE_APPLICATION,
			other = GL_DEBUG_SOURCE_OTHER,
			dontCare = GL_DONT_CARE
		};

		enum class ErrorLevelType
		{
			error = GL_DEBUG_TYPE_ERROR,
			deprecatedBehavior = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
			undefinedBehavior = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
			portability = GL_DEBUG_TYPE_PORTABILITY,
			performance = GL_DEBUG_TYPE_PERFORMANCE,
			marker = GL_DEBUG_TYPE_MARKER,
			pushGroup = GL_DEBUG_TYPE_PUSH_GROUP,
			popGroup = GL_DEBUG_TYPE_POP_GROUP,
			other = GL_DEBUG_TYPE_OTHER,
			dontCare = GL_DONT_CARE
		};

		enum class SeverityType
		{
			low = GL_DEBUG_SEVERITY_LOW,
			medium = GL_DEBUG_SEVERITY_MEDIUM,
			high = GL_DEBUG_SEVERITY_HIGH,
			notification = GL_DEBUG_SEVERITY_NOTIFICATION,
			dontCare = GL_DONT_CARE
		};

		SourceType sourceType = SourceType::dontCare;
		ErrorLevelType errorLevelType = ErrorLevelType::dontCare;
		SeverityType everityType = SeverityType::dontCare;
	};

	struct GLErrorDescription
	{
		GLError error;
		std::string description;
	};

	struct GLErrorDescription2
	{
		std::string description;
		GLErrorFilter::SourceType sourceType = GLErrorFilter::SourceType::dontCare;
		GLErrorFilter::ErrorLevelType errorLevelType = GLErrorFilter::ErrorLevelType::dontCare;
		GLErrorFilter::SeverityType severityType = GLErrorFilter::SeverityType::dontCare;
	};

	class GLErrorChecker
	{
	public:
		static GLStateInfo debugState() noexcept;
		static std::optional<GLErrorDescription> getError() noexcept;

		static void debugMessageCallback(GLenum source,
			GLenum type,
			unsigned int id,
			GLenum severity,
			GLsizei length,
			const char *message,
			const void *userParam);

		static void enableDebugMessageCallback(const GLErrorFilter* filter, const bool flag) noexcept;

		static bool isNoError() noexcept;
	};


}

namespace ks
{
	class GLTextureReader
	{
	public:
		static void read(ks::PixelBuffer& pixelBuffer) noexcept;
	};

}


#endif // !KSRENDERENGINEOPENGLUTILHPP
