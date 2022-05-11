#include "OpenGL/OpenGLInit.hpp"
#include <assert.h>
#include <mutex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ks
{
	
	void GLInit()
	{
		static std::once_flag onceFlag;
		std::call_once(onceFlag, []
		{
			static GLFWwindow* glfwWindow = nullptr; // TODO:
			int status = glfwInit();
			assert(status);
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

			glfwWindow = glfwCreateWindow(200, 200, "OpenGL", NULL, NULL);
			assert(glfwWindow);
			glfwMakeContextCurrent(glfwWindow);
			
			status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			assert(status);
		});
	}

}