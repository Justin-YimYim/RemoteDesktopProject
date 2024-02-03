#pragma once

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

		inline ID3D11Device* GetD3D11Device() const {
			return mD3D11Device.Get();
		}
		inline ID3D11DeviceContext* GetD3D11DeviceContext() const {
			return mD3d11DeviceContext.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mD3D11Device{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3d11DeviceContext;

		void CreateD3D11Devices() {

		}
	};

	inline D3D11RenderContext* GetD3D11RenderContext() {
		static std::unique_ptr<D3D11RenderContext> renderContext{ std::make_unique<D3D11RenderContext>() };
		
		return renderContext.get();
	}
}

#endif
