#pragma once

#ifndef WINDOW_CLASS
#define WINDOW_CLASS

#include "pch.hpp"
#include "nonCopyable.hpp"
#include "Graphic/GraphicInterface.hpp"

// run procedure:
// 1. GetWindowClass
// 2. Create a Window based on the info, that runs MasterWndProc which call WndProc
// a. In create, it will CreateWindowExW()
// b. CreateWindowExW will auto run MasterWndProc

namespace RemoteDesk {
	class WindowClass : public nonCopyable<WindowClass> {
	public:
		WindowClass(const D3D11RenderContext* renderContext = GetD3D11RenderContext()) {
			if (renderContext == nullptr) throw std::invalid_argument{ "Render Context must not be null." };

			mRenderContext = renderContext;
		}

		WindowClass(WindowClass&& window) noexcept : mHandle{ window.mHandle } {
			window.mHandle = nullptr;
			mRenderContext = window.mRenderContext;
			mRenderTarget = std::move(window.mRenderTarget);
		}

		WindowClass& operator=(WindowClass&& rhs) noexcept {
			if (mHandle) this->Destroy();
			mHandle = std::exchange(rhs.mHandle, nullptr);
			mRenderContext = rhs.mRenderContext;
			mRenderTarget = std::move(rhs.mRenderTarget);
			return *this;
		}

		void Show() {
			std::ignore = ShowWindow(mHandle, SW_SHOW);
			std::ignore = UpdateWindow(mHandle);
		}

		void Hide() {
			std::ignore = ShowWindow(mHandle, SW_HIDE);
			std::ignore = UpdateWindow(mHandle);
		}

		void Minimize() {
			std::ignore = ShowWindow(mHandle, SW_MINIMIZE);
			std::ignore = UpdateWindow(mHandle);
		}

		void Restore() {
			std::ignore = ShowWindow(mHandle, SW_NORMAL);
			std::ignore = UpdateWindow(mHandle);
		}

		void Maximize() {
			std::ignore = ShowWindow(mHandle, SW_MAXIMIZE);
			std::ignore = UpdateWindow(mHandle);
		}

		bool IsMinimized() const {
			return IsIconic(mHandle) != FALSE;
		}

		bool IsMaximized() const {
			return IsZoomed(mHandle) != FALSE;
		}

		HWND GetHandle() const {
			return mHandle;
		}

		virtual int Run() = 0;

	protected:
		WindowClass() = default;

		// Get window class information/ init a wcex info
		virtual WNDCLASSEXW GetWindowClass() const {
			WNDCLASSEXW wcex{};
			
			// GetModuleHandle(nullptr)会返回exe中的HINSTANCE
			wcex.hInstance = GetModuleHandleW(nullptr);
			wcex.cbSize = sizeof(wcex);
			// Procedure runs in this windows
			wcex.lpfnWndProc = MasterWndProc;
			// Make Cursor as normal cursor
			wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);

			return wcex;
		}

		// the process would be ran in this window class
		virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
			case WM_DESTROY:
				PostQuitMessage(EXIT_SUCCESS);
				break;
			case WM_SIZE:
			{
				// OutputDebugStringA("Ran WM_SIZE");
				mRenderContext->GetD3D11DeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);
				mRenderTarget->OnWindowResized();

				ID3D11RenderTargetView* renderTargets[] = { mRenderTarget->GetRenderTargetView() };

				mRenderContext->GetD3D11DeviceContext()->OMSetRenderTargets(ARRAYSIZE(renderTargets), renderTargets, nullptr);
				break;
			}
			default: 
				return DefWindowProcW(mHandle, msg, wParam, lParam);
			}

			return LRESULT{ 0 };
		}

		// Create a real Window screen
		// During Create it will automatically create a unique D3D11RenderTarget
		void Create(
			const std::wstring& title = L"",
			int width = 800,
			int height = 600,
			int x = 0,
			int y = 0,
			DWORD style = WS_OVERLAPPEDWINDOW,
			DWORD styleEx = 0
		) {
			WNDCLASSEXW wcex{ this->GetWindowClass() };
			
			if (!RegisterClassExW(&wcex) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) 
				throw std::system_error{ static_cast<int>(GetLastError()), std::system_category() };
		
			sInitializer = this;

			std::ignore = CreateWindowExW(
				styleEx,
				wcex.lpszClassName,
				title.c_str(),
				style,
				x,
				y,
				width,
				height,
				nullptr,
				nullptr,
				wcex.hInstance,
				nullptr
			);

			if (mHandle == nullptr)
				throw std::runtime_error{ "Fail to create window." };

			mRenderTarget = std::make_unique<D3D11RenderTarget>(mHandle, mRenderContext);
		}

		inline void Destroy() {
			if (mHandle) DestroyWindow(mHandle);
		}

		D3D11RenderTarget* GetRenderTarget() {
			return mRenderTarget.get();
		}

		const D3D11RenderContext* GetRenderContext() {
			return mRenderContext;
		}

	private:
		HWND mHandle{ nullptr };
		std::unique_ptr<D3D11RenderTarget> mRenderTarget{ };
		const D3D11RenderContext* mRenderContext{ };

		inline static WindowClass* sInitializer{ nullptr };

		// An encapsulation for WindowProc function
		static LRESULT CALLBACK MasterWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			// GetWindowLongPtrW: Retrieves information about the specified window
			// Based on a hWnd (window handler) and offset GWLP_USERDATA
			WindowClass* window = reinterpret_cast<WindowClass*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

			if (!window) {
				// if initializer not set return default window procedure
				if (sInitializer == nullptr) return DefWindowProcW(hWnd, msg, wParam, lParam);

				window = sInitializer;
				SetWindowLongW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			}


			// WM_NCCREATE: when a window is first created.
			if (msg == WM_NCCREATE) window->mHandle = hWnd;
			
			const LRESULT result = window->WndProc(msg, wParam, lParam);
			
			// WM_NCDESTROY: when a window is destroyed
			if (msg == WM_NCDESTROY) window->mHandle = nullptr;
			
			return result;
		}

	};
}


#endif
