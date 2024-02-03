#pragma once

#ifndef SELECTION_WINDOW
#define SELECTION_WINDOW

#include "pch.hpp"
#include "WindowClass.hpp"

namespace RemoteDesk {

	class SelectionWindow : public WindowClass {
	public:
		SelectionWindow() {
			this->Create();

			this->Show();
		}
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