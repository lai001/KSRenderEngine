#include <iostream>
#include <string>
#include <functional>
#include <KSRenderEngine/KSRenderEngine.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <stb_image_write.h>

GLFWwindow* Window = nullptr;

struct DataSource
{
	bool isEnableSaveImage = false;
} dataSource;

void glInit()
{
	const bool initRet = glfwInit();
	assert(initRet);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	Window = glfwCreateWindow(1280, 720, "Example", NULL, NULL);
	assert(Window);

	glfwSwapInterval(1);

	glfwMakeContextCurrent(Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_MULTISAMPLE);
}

void imguiInit()
{
	const std::string Version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	const bool initRet = ImGui_ImplOpenGL3_Init(Version.c_str());
	assert(initRet);
}

void imguiDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void imguiDrawUI()
{
	ImGuiWindowFlags Flags = 0;
	Flags |= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowBgAlpha(0.2);
	ImGui::Begin("Example", nullptr, Flags);
	ImGui::Checkbox("Is enable save image", &dataSource.isEnableSaveImage);

	ImGui::End();
}

void imaguiTick()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	imguiDrawUI();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void frameTick()
{
	const std::string vert =
R"(
	#version 330 core
	layout (location = 0) in vec2 aPosition;
	layout (location = 1) in vec4 aColor;
	out vec4 oColor;
	void main()
	{
		gl_Position = vec4(aPosition.xy, 0.0, 1.0);
		oColor = aColor;
	}
)";

	const std::string frag =
R"(
	#version 330 core
	in vec4 oColor;
	void main()
	{
		gl_FragColor = oColor;
	}
)";

	static ks::RenderEngine engine = ks::RenderEngine(ks::RenderEngine::RendererType::OpenGL);

	struct Vertex
	{
		glm::vec2 aPosition;
		glm::vec4 aColor;
	};

	ks::VertexBufferLayout layout = ks::VertexBufferLayout().f32(2).f32(4);

	std::vector<unsigned int> indexBufferData = { 0, 1, 2, 2, 1, 3 };

	std::vector<Vertex> vertexBuffer;
	{
		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;

		topLeft.aPosition = glm::vec2(-1.0, 1.0);
		topLeft.aColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		topRight.aPosition = glm::vec2(1.0, 1.0);
		topRight.aColor = glm::vec4(0.0, 1.0, 0.0, 1.0);
		bottomLeft.aPosition = glm::vec2(-1.0, -1.0);
		bottomLeft.aColor = glm::vec4(0.0, 1.0, 1.0, 1.0);
		bottomRight.aPosition = glm::vec2(1.0, -1.0);
		bottomRight.aColor = glm::vec4(0.0, 0.0, 1.0, 1.0);

		vertexBuffer.push_back(topLeft);
		vertexBuffer.push_back(topRight);
		vertexBuffer.push_back(bottomLeft);
		vertexBuffer.push_back(bottomRight);
	}
	static ks::PixelBuffer* pixelBufferPtr = new ks::PixelBuffer(1000, 1000, ks::PixelBuffer::FormatType::rgba8);
	ks::PixelBuffer& pixelBuffer = *pixelBufferPtr;
	ks::IShader* shader = engine.createShader(vert, frag, { });
	ks::IFrameBuffer* frameBuffer = engine.createFrameBuffer(1000, 1000);
	ks::IRenderBuffer * renderBuffer = engine.createRenderBuffer(vertexBuffer.data(), sizeof(Vertex) * vertexBuffer.size(), layout, indexBufferData.data(), indexBufferData.size());
	renderBuffer->setFrameBuffer(*frameBuffer);
	renderBuffer->setShader(*shader);
	renderBuffer->commit();

	engine.readTexture(frameBuffer, pixelBuffer);

	if (dataSource.isEnableSaveImage)
	{
		unsigned char* data = reinterpret_cast<unsigned char*>(pixelBuffer.getMutableData()[0]);
		std::string targetPath = fmt::format("{}/{}.png", ks::Application::getAppDir(), "KSImage");
		int writeStatus = stbi_write_png(targetPath.c_str(), pixelBuffer.getWidth(), pixelBuffer.getHeight(), pixelBuffer.getChannels(), data, pixelBuffer.getWidth() * pixelBuffer.getChannels());
		spdlog::info(targetPath);
	}

	engine.erase(shader);
	engine.erase(renderBuffer);
	engine.erase(frameBuffer);
}

int main(int argc, char** argv)
{
	ks::Application::Init(argc, argv);
	glInit();
	imguiInit();
	defer
	{
		imguiDestroy();
		glfwTerminate();
	};

	while (!glfwWindowShouldClose(Window))
	{
		frameTick();
		imaguiTick();
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	return 0;
}
