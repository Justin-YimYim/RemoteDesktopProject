#pragma once

// This is the swap chain
#ifndef D3D11RENDERTARGET
#define D3D11RendERTARGET

#include "pch.hpp"
#include "Window/WindowClass.hpp"
#include "D3D11RenderContext.hpp"

namespace RemoteDesk {
	class D3D11RenderTarget {
		public:
			D3D11RenderTarget(const WindowClass& window, const D3D11RenderContext* renderContext = GetD3D11RenderContext()) :
				mRenderContext{ renderContext }
			{
				if (renderContext == nullptr) throw std::invalid_argument{ "Render context must not be nullptr" };

				auto dxgiFactory = mRenderContext->QueryDXGIFactory();


				HWND windowHandle = window.GetHandle();
				RECT rc{};
				GetClientRect(windowHandle, &rc);

				const UINT clientWidth = static_cast<UINT>(rc.right - rc.left);
				const UINT clientHeight = static_cast<UINT>(rc.bottom - rc.top);

				DXGI_SWAP_CHAIN_DESC scd{};

				scd.BufferCount = 2;
				scd.BufferDesc.Width = clientWidth;
				scd.BufferDesc.Height = clientHeight;
				scd.BufferDesc.RefreshRate = { 60, 1 };
				// RGBA all 8 bits and use unsigned value
				scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				// 如果值为true将会已窗口模式显示；全屏则为false;
				scd.Windowed = true;
				scd.SampleDesc = { 1,0 };
				scd.OutputWindow = windowHandle;
				// 描述后台缓存的表面用法和CPU访问设置
				scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

				HRESULT hr = dxgiFactory->CreateSwapChain(mRenderContext->GetD3D11Device(), &scd, &mSwapChain);

				if (FAILED(hr)) throw std::runtime_error{ "Failed to create swap chain." };

				this->CreateBufferAndView();
				}

			IDXGISwapChain* GetSwapChain() const {
				return mSwapChain.Get();
			}

			ID3D11Texture2D* GetBackBuffer() const {
				return mBackBuffer.Get();
			}

			ID3D11RenderTargetView* GetRenderTargetView() const {
				return mRenderTargetView.Get();
			}

			void OnWindowResized() {
				mBackBuffer.Reset();
				mRenderTargetView.Reset();

				// If height/ width = 0, DXGI will use the client area of the target window. 
				mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				this->CreateBufferAndView();
			}

			HRESULT Present(UINT vSync = 0) {
				return mSwapChain->Present(vSync, 0);
			}

		private:
			const D3D11RenderContext* mRenderContext{ nullptr };

			Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	
			void CreateBufferAndView() {
				HRESULT hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&mBackBuffer));

				if (FAILED(hr)) throw std::runtime_error{ "Failed to get back buffer from swap chain." };

				// mullptr has to change if render 3d
				hr = mRenderContext->GetD3D11Device()->CreateRenderTargetView(mBackBuffer.Get(), nullptr, &mRenderTargetView);

				if (FAILED(hr)) throw std::runtime_error{ "Failed to create render target view from back buffer." };

			}
	};
}

#endif // !D3D11RENDERTARGET

