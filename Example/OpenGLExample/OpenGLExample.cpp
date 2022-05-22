#include <iostream>
#include <string>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <KSRenderEngine/KSRenderEngine.hpp>
#include "ImageIO.hpp"

GLFWwindow* Window = nullptr;

struct DataSource
{
	bool isEnableSaveImage = false;
	float intensity = 1.0;
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

	Window = glfwCreateWindow(1280, 720, "OpenGLExample", NULL, NULL);
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
	ImGui::SliderFloat("Intensity", &dataSource.intensity, 0.0, 1.0);
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
struct VS_INPUT
{
    float2 pos : POSITION;
	float2 texCoord : TEXCOORD;
    float4 col : COLOR0;
};
            
struct PS_INPUT
{
    float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
    float4 col : COLOR0;
};
            
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos.xy, 0.0f, 1.0f);
    output.col = input.col;
	output.texCoord = input.texCoord;
    return output;
}
)";

	const std::string frag =
		R"(
cbuffer Uniforms : register(b0)
{
    float intensity;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
    float4 col : COLOR0;
};

Texture2D colorMap: register(t0);
SamplerState colorMapSampler: register(s0);

float4 main(PS_INPUT vertexOut) : SV_Target
{
	float4 color = lerp(vertexOut.col * intensity, colorMap.Sample(colorMapSampler, vertexOut.texCoord), 0.5f);
	return color;
}
)";

	std::unique_ptr < ks::PixelBuffer > imageData = ImageIO::readImageFromFilePath2(ks::Application::getResourcePath("maple.jpg"));

	static ks::IRenderEngine* enginePtr = ks::RenderEngine::create(ks::GLRenderEngineCreateInfo());
	ks::IRenderEngine& engine = *enginePtr;

	struct Vertex
	{
		glm::vec2 aPosition;
		glm::vec2 aTexCoord;
		glm::vec4 aColor;
	};

	ks::VertexBufferLayout layout = ks::VertexBufferLayout().f32(2).f32(2).f32(4);

	std::vector<unsigned int> indexBufferData = { 0, 1, 2, 2, 1, 3 };

	std::vector<Vertex> vertexBuffer;
	{
		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;

		topLeft.aPosition = glm::vec2(-1.0, 1.0);
		topLeft.aColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		topLeft.aTexCoord = glm::vec2(0.0, 1.0);

		topRight.aPosition = glm::vec2(1.0, 1.0);
		topRight.aColor = glm::vec4(0.0, 1.0, 0.0, 1.0);
		topRight.aTexCoord = glm::vec2(1.0, 1.0);

		bottomLeft.aPosition = glm::vec2(-1.0, -1.0);
		bottomLeft.aColor = glm::vec4(0.0, 1.0, 1.0, 1.0);
		bottomLeft.aTexCoord = glm::vec2(0.0, 0.0);

		bottomRight.aPosition = glm::vec2(1.0, -1.0);
		bottomRight.aColor = glm::vec4(0.0, 0.0, 1.0, 1.0);
		bottomRight.aTexCoord = glm::vec2(1.0, 0.0);

		vertexBuffer.push_back(topLeft);
		vertexBuffer.push_back(topRight);
		vertexBuffer.push_back(bottomLeft);
		vertexBuffer.push_back(bottomRight);
	}

	static ks::PixelBuffer* pixelBufferPtr = new ks::PixelBuffer(1000, 1000, ks::PixelBuffer::FormatType::rgba8);
	ks::PixelBuffer& pixelBuffer = *pixelBufferPtr;
	ks::IFrameBuffer* frameBuffer = engine.createFrameBuffer(1000, 1000);
	ks::ITexture2D* colorMap = engine.createTexture2D(imageData->getWidth(), imageData->getHeight(), ks::TextureFormat::R8G8B8A8_UNORM, imageData->getImmutableData()[0]);
	ks::IShader* shader = engine.createShader(vert, frag);
	shader->bind();
	shader->setUniform("Uniforms.intensity", ks::UniformValue(dataSource.intensity));
	shader->setTexture2D("colorMap", *colorMap);
	ks::IRenderBuffer * renderBuffer = engine.createRenderBuffer(vertexBuffer.data(), vertexBuffer.size(), sizeof(Vertex),
		layout,
		indexBufferData.data(), indexBufferData.size(), ks::IIndexBuffer::IndexDataType::uint32);
	renderBuffer->setClearColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	renderBuffer->setViewport(0, 0, 1000, 1000);
	renderBuffer->setClearBufferFlags(ks::ClearBufferFlags::color);
	renderBuffer->setShader(*shader);

	renderBuffer->commit(*frameBuffer);

	engine.readTexture(frameBuffer, pixelBuffer);

	if (dataSource.isEnableSaveImage)
	{
		const std::string targetPath = fmt::format("{}/{}.png", ks::Application::getAppDir(), "OpenGLExample");
		const bool writeStatus = ImageIO::saveImage(pixelBuffer, targetPath);
		spdlog::info("{}, {}", bool(writeStatus), targetPath);
	}

	engine.erase(shader);
	engine.erase(renderBuffer);
	engine.erase(frameBuffer);
	engine.erase(colorMap);
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
