#include <iostream>
#include <string>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include <KSRenderEngine/KSRenderEngine.hpp>
#include "ImageIO.hpp"

struct DataSource
{
	bool isEnableSaveImage = false;
	float intensity = 1.0;
} dataSource;

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void frameTick();

int main(int argc, char** argv)
{
	ks::Application::Init(argc, argv);
	spdlog::set_level(spdlog::level::debug);
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("D3D11Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("D3D11Example");
			ImGui::Checkbox("Is enable save image", &dataSource.isEnableSaveImage);
			ImGui::SliderFloat("Intensity", &dataSource.intensity, 0.0, 1.0);
			ImGui::End();
		}

		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		frameTick();
		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	createDeviceFlags = createDeviceFlags | D3D11_CREATE_DEVICE_DEBUG;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
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

	ks::D3D11RenderEngineCreateInfo createInfo;
	createInfo.device = g_pd3dDevice;
	createInfo.context = g_pd3dDeviceContext;

	static ks::IRenderEngine* enginePtr = ks::RenderEngine::create(createInfo);
	ks::IRenderEngine& engine = *enginePtr;

	struct Vertex
	{
		glm::vec2 aPosition;
		glm::vec2 texCoord;
		glm::vec4 aColor;
	};

	ks::VertexBufferLayout layout = ks::VertexBufferLayout()
		.f32(2, ks::VertexBufferElement::Semantics::position)
		.f32(2, ks::VertexBufferElement::Semantics::texcoord)
		.f32(4, ks::VertexBufferElement::Semantics::color);

	std::vector<unsigned int> indexBufferData = { 0, 1, 2, 2, 1, 3 };

	std::vector<Vertex> vertexBuffer;
	{
		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;

		topLeft.aPosition = glm::vec2(-1.0, 1.0);
		topLeft.aColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		topLeft.texCoord = glm::vec2(0.0, 0.0);
		topRight.aPosition = glm::vec2(1.0, 1.0);
		topRight.aColor = glm::vec4(0.0, 1.0, 0.0, 1.0);
		topRight.texCoord = glm::vec2(1.0, 0.0);
		bottomLeft.aPosition = glm::vec2(-1.0, -1.0);
		bottomLeft.aColor = glm::vec4(0.0, 1.0, 1.0, 1.0);
		bottomLeft.texCoord = glm::vec2(0.0, 1.0);
		bottomRight.aPosition = glm::vec2(1.0, -1.0);
		bottomRight.aColor = glm::vec4(0.0, 0.0, 1.0, 1.0);
		bottomRight.texCoord = glm::vec2(1.0, 1.0);

		vertexBuffer.push_back(topLeft);
		vertexBuffer.push_back(topRight);
		vertexBuffer.push_back(bottomLeft);
		vertexBuffer.push_back(bottomRight);
	}

	static ks::PixelBuffer* pixelBufferPtr = new ks::PixelBuffer(1000, 1000, ks::PixelBuffer::FormatType::rgba8);
	ks::PixelBuffer& pixelBuffer = *pixelBufferPtr;
	ks::ITexture2D* colorMap = engine.createTexture2D(imageData->getWidth(), imageData->getHeight(), ks::TextureFormat::R8G8B8A8_UNORM, imageData->getImmutableData()[0]);
	ks::IFrameBuffer* frameBuffer = engine.createFrameBuffer(1000, 1000);
	ks::IShader* shader = engine.createShader(vert, frag);
	shader->bind();
	shader->setUniform("Uniforms.intensity", ks::UniformValue(dataSource.intensity));
	shader->setTexture2D("colorMap", *colorMap);
	ks::IRenderBuffer * renderBuffer = engine.createRenderBuffer(vertexBuffer.data(), vertexBuffer.size(), sizeof(Vertex),
		layout,
		indexBufferData.data(), indexBufferData.size(), ks::IIndexBuffer::IndexDataType::uint32);
	ks::IBlendState* blendState = engine.createBlendState(ks::BlendStateDescription::Addition::getDefault(), ks::BlendStateDescription::getDefault());
	ks::IDepthStencilState* depthStencilState = engine.createDepthStencilState(ks::DepthStencilStateDescription::getDefault());
	ks::IRasterizerState* rasterizerState = engine.createRasterizerState(ks::RasterizerStateDescription::getDefault());
	renderBuffer->setClearColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	renderBuffer->setViewport(0, 0, 1000, 1000);
	renderBuffer->setClearBufferFlags(ks::ClearBufferFlags::color);
	renderBuffer->setShader(*shader);
	renderBuffer->setBlendState(*blendState);
	renderBuffer->setDepthStencilState(*depthStencilState);
	renderBuffer->setRasterizerState(*rasterizerState);
	renderBuffer->setPrimitiveTopologyType(ks::PrimitiveTopologyType::trianglelist);
	renderBuffer->commit(*frameBuffer);

	if (dataSource.isEnableSaveImage)
	{
		engine.readTexture(frameBuffer, pixelBuffer);
		const std::string targetPath = fmt::format("{}/{}.png", ks::Application::getAppDir(), "D3D11Example");
		const bool writeStatus = ImageIO::saveImage(pixelBuffer, targetPath);
		spdlog::info("{}, {}", bool(writeStatus), targetPath);
	}

	engine.erase(shader);
	engine.erase(frameBuffer);
	engine.erase(renderBuffer);
	engine.erase(blendState);
	engine.erase(depthStencilState);
	engine.erase(rasterizerState);
	engine.erase(colorMap);
}
