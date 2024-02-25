#pragma once

#ifndef SELECTION_WINDOW
#define SELECTION_WINDOW

#include "pch.hpp"
#include "WindowClass.hpp"

namespace RemoteDesk {

	class SelectionWindow : public WindowClass {
	public:
		SelectionWindow(const D3D11RenderContext* renderContext)
			: WindowClass (renderContext)
		{
			this->Create();

			this->Show();
		}

		int Run() override {
			this->Show();

			MSG msg{};

			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				else {
					auto renderContext = this->GetRenderContext();
					auto renderTarget = this->GetRenderTarget();

					renderContext->GetD3D11DeviceContext()->ClearRenderTargetView(renderTarget->GetRenderTargetView(), DirectX::Colors::CornflowerBlue);
					renderTarget->Present();

				}
			}

			return static_cast<int>(msg.wParam);
		};
	private:
		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch (msg) 
			{
				case WM_DESTROY:
					PostQuitMessage(EXIT_SUCCESS);
					break;
				default:
					return WindowClass::WndProc(msg, wParam, lParam);
			}
			return LRESULT{ 0 };
		}

		WNDCLASSEXW GetWindowClass() const override {
			auto wcex{ WindowClass::GetWindowClass() };
				
			wcex.lpszClassName = L"SelectionWindow";

			return wcex;
		}
	};

}
#endif