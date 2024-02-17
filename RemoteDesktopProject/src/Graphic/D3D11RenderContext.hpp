#pragma once

// Render Context is to access the hardware
#ifndef D3D11RENDER
#define D3D11RENDER

#include "pch.hpp"

namespace RemoteDesk {
	// Initialization of D3D11
	class D3D11RenderContext {
	public:
		D3D11RenderContext() {
			this->CreateD3D11Devices();
		}

		// implements methods for generating DXGI objects
		inline Microsoft::WRL::ComPtr<IDXGIFactory> QueryDXGIFactory() const {
			using Microsoft::WRL::ComPtr;

			// https://blog.csdn.net/MaxLykoS/article/details/102527014
			// Lowest Level Resource Creation Mechanism
			ComPtr<IDXGIDevice> dxgiDevice{};
			HRESULT hr = mD3D11Device.As(&dxgiDevice);

			if (FAILED(hr)) throw std::runtime_error{ "Failed to query DXGI device." };

			// Communication With The Hardware 
			ComPtr<IDXGIAdapter> dxgiAdapter{};
			hr = dxgiDevice->GetAdapter(&dxgiAdapter);

			if (FAILED(hr)) throw std::runtime_error{ "Failed to get DXGI adaptor from DXGI device" };

			ComPtr<IDXGIFactory> dxgiFactory{};
			hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

			if (FAILED(hr)) throw std::runtime_error{ "Failed to get DXGI factory from DXGI adapter." };

			return dxgiFactory;
		}
		
		inline ID3D11Device* GetD3D11Device() const {
			return mD3D11Device.Get();
		}

		inline ID3D11DeviceContext* GetD3D11DeviceContext() const {
			return mD3d11DeviceContext.Get();
		}

	private:
		// 接口用于检测显示适配器功能和分配资源
		Microsoft::WRL::ComPtr<ID3D11Device> mD3D11Device{};
		// 接口用于设置管线状态、将资源绑定到图形管线和生成渲染命令
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3d11DeviceContext;

		void CreateD3D11Devices() {
			D3D_FEATURE_LEVEL requestedFeatureLevels[] = {
				D3D_FEATURE_LEVEL_11_0
			};

			D3D_FEATURE_LEVEL featureLevel{};
		
			HRESULT hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				requestedFeatureLevels,
				ARRAYSIZE(requestedFeatureLevels),
				D3D11_SDK_VERSION,
				&mD3D11Device,
				&featureLevel,
				&mD3d11DeviceContext);
			if (FAILED(hr)) 
				throw std::system_error{static_cast<int>(hr), std::system_category()};
		
			if (featureLevel < D3D_FEATURE_LEVEL_11_0)
				throw std::runtime_error{"D3D Level < 11"};
		}
	};

	inline D3D11RenderContext* GetD3D11RenderContext() {
		static std::unique_ptr<D3D11RenderContext> renderContext{ std::make_unique<D3D11RenderContext>() };
		
		return renderContext.get();
	}
}

#endif
