#pragma once

#ifndef DESKTOPSCREENLOADER
#define DESKTOPSCREENLOADER

#include "pch.hpp"
#include "D3D11RenderContext.hpp"
#include "NonCopyable.hpp"

namespace RemoteDesk {
	class DesktopScreenLoader final: nonCopyable<DesktopScreenLoader> {
	public:
		DesktopScreenLoader(const D3D11RenderContext* renderContext = GetD3D11RenderContext()) :
			mRenderContext{ renderContext } 
		{
			if (renderContext == nullptr) throw std::invalid_argument{ "Render context must not be nullptr" };
			
			mDXGIAdapter = mRenderContext->QueryDXGIAdapter();

			// 获取DXGI output
			IDXGIOutput* dxgiOutput;
			HRESULT hr = mDXGIAdapter->EnumOutputs(0, &dxgiOutput);
			if (FAILED(hr)) throw std::runtime_error{ "Failed to get DXGI Output from DXGI adapter." };

			// 获取DXGI output1  
			IDXGIOutput1* dxgiOutput1;
			hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&dxgiOutput1));
			dxgiOutput->Release();
			dxgiOutput = nullptr;
			if (FAILED(hr)) throw std::runtime_error{ "Failed to get DXGI Output1 from dxgiOutput" };

			// 获取DXGI OutputDuplication    
			hr = dxgiOutput1->DuplicateOutput(mRenderContext->GetD3D11Device(), &mDupl);
			dxgiOutput1->Release();
			dxgiOutput1 = nullptr;
			if (FAILED(hr)) throw std::runtime_error{ "Failed to get DXGIOutputDuplication from dxgiOutput" };
		

			IDXGIResource* deskRes = nullptr;
			DXGI_OUTDUPL_FRAME_INFO frameInfo;
			hr = mDupl->AcquireNextFrame(0, &frameInfo, &deskRes);
			if (FAILED(hr)) throw std::runtime_error{ "Failed to acquireNextFrame." };

			hr = deskRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&mDesktopTexture2d);
			deskRes->Release();
			deskRes = nullptr;
			if (FAILED(hr)) throw std::runtime_error{ "Failed to create texture 2d" };

			mDesktopTexture2d->GetDesc(&mDesktopDesc);
			mDesktopDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			mDesktopDesc.Usage = D3D11_USAGE_STAGING;
			mDesktopDesc.BindFlags = 0;
			mDesktopDesc.MiscFlags = 0; // D3D11_RESOURCE_MISC_GDI_COMPATIBLE ?
			hr = mRenderContext->GetD3D11Device()->CreateTexture2D(&mDesktopDesc, nullptr, &mResultTexture);

			if (SUCCEEDED(hr)) mRenderContext->GetD3D11DeviceContext()->CopyResource(mResultTexture.Get(), mDesktopTexture2d.Get());
			else throw std::runtime_error{"Unable to copy texture 2d"};
			

		}

		inline D3D11_TEXTURE2D_DESC GetDesktopDesc() const {
			return mDesktopDesc;
		}

		inline ID3D11Texture2D* GetTexture2d() const {
			return mResultTexture.Get();
		}
	private:
		const D3D11RenderContext* mRenderContext{ nullptr };
		Microsoft::WRL::ComPtr<IDXGIAdapter> mDXGIAdapter;
		Microsoft::WRL::ComPtr<IDXGIOutputDuplication> mDupl;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDesktopTexture2d;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mResultTexture;
		D3D11_TEXTURE2D_DESC mDesktopDesc;
	};

	inline DesktopScreenLoader* GetDesktopScreenLoader() {
		static std::unique_ptr<DesktopScreenLoader> desktopScreenLoader{std::make_unique<DesktopScreenLoader>()};

		return desktopScreenLoader.get();
	}
}


#endif