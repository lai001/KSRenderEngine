#ifndef KSRenderEngine_D3D11_D3D11Init_hpp
#define KSRenderEngine_D3D11_D3D11Init_hpp

#ifndef _WIN32
#error
#endif // !_WIN32

#include <string>
#include <functional>
#include "Platform/windows.hpp"

namespace ks
{
	class D3D11Window
	{
	public:
		struct Configuration
		{
			std::string windowName = "";
			unsigned int windowWidth = 1280;
			unsigned int windowHeight = 720;
			int x = 0;
			int y = 0;
			bool isDebugEnable = true;
		};

		enum class ShowWindowCommandType
		{
			hide,
			showNormal,
			normal,
			showMinimized,
			showMaximized,
			maximize,
			showNoActivate,
			show,
			minimize,
			showMinnoactive,
			showNa,
			restore,
			showDefault,
			forceMinimize,
			max
		};

	public:
		explicit D3D11Window(const Configuration &cfg);
		~D3D11Window();

		std::function<bool(HWND, UINT, WPARAM, LPARAM, bool&)> WndProcCallback;
		std::function<void(unsigned int width, unsigned int height)> sizeChange;

		bool shouldClose();
		void present(const int vsync = 1) const noexcept;
		void setRenderTarget() const noexcept;
		void clearColor();

		ID3D11Device* getDevice() const noexcept;
		ID3D11DeviceContext* getDeviceContext() const noexcept;

		bool showWindow(const ShowWindowCommandType type);
		bool updateWindow() const noexcept;

	private:
		Configuration cfg;
		HWND hwnd = nullptr;
		WNDCLASSEX wc;
		ID3D11Device*            pd3dDevice = nullptr;
		ID3D11DeviceContext*     pd3dDeviceContext = nullptr;
		IDXGISwapChain*          pSwapChain = nullptr;
		ID3D11RenderTargetView*  mainRenderTargetView = nullptr;

		bool createDeviceD3D(HWND hWnd);
		void cleanupDeviceD3D();
		void createRenderTarget();
		void cleanupRenderTarget();
		void Init(const Configuration &cfg);

		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif // !KSRenderEngine_D3D11_D3D11Init_hpp