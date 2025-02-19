#pragma once

#include "RefCounter.hpp"
#include "SwapChainWrapper1.hpp"
#include "WrapperPointer.hpp"
#include <d2d1_2.h>
#include <dwrite.h>
#include <dxgi1_2.h>
#include <memory>
#include <wrl/client.h>

namespace DxWrappers {

using namespace Microsoft::WRL;

class DXGIFactoryWrapper2 : public IDXGIFactory2 {
	RefCounter _refCounter;
	ComPtr<IDXGIFactory2> _target;
	ComPtr<ID2D1Factory2> _D2DFactory;
	WrapperPointer<DXGISwapChainWrapper1> _currentSwapChain;

public:
	DXGIFactoryWrapper2(ComPtr<IDXGIFactory2> target);
	virtual ~DXGIFactoryWrapper2();
	void toggleDvdMode(bool enabled);

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall SetPrivateData(REFGUID Name, UINT DataSize, const void *pData) override;
	virtual HRESULT __stdcall SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown) override;
	virtual HRESULT __stdcall GetPrivateData(REFGUID Name, UINT *pDataSize, void *pData) override;
	virtual HRESULT __stdcall GetParent(REFIID riid, void **ppParent) override;
	virtual HRESULT __stdcall EnumAdapters(UINT Adapter, IDXGIAdapter **ppAdapter) override;
	virtual HRESULT __stdcall MakeWindowAssociation(HWND WindowHandle, UINT Flags) override;
	virtual HRESULT __stdcall GetWindowAssociation(HWND *pWindowHandle) override;
	virtual HRESULT __stdcall CreateSwapChain(
			IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain
	) override;
	virtual HRESULT __stdcall CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter **ppAdapter) override;
	virtual HRESULT __stdcall EnumAdapters1(UINT Adapter, IDXGIAdapter1 **ppAdapter) override;
	virtual BOOL __stdcall IsCurrent(void) override;
	virtual BOOL __stdcall IsWindowedStereoEnabled(void) override;
	virtual HRESULT __stdcall CreateSwapChainForHwnd(
			IUnknown *pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc,
			const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc, IDXGIOutput *pRestrictToOutput,
			IDXGISwapChain1 **ppSwapChain
	) override;
	virtual HRESULT __stdcall CreateSwapChainForCoreWindow(
			IUnknown *pDevice, IUnknown *pWindow, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput,
			IDXGISwapChain1 **ppSwapChain
	) override;
	virtual HRESULT __stdcall GetSharedResourceAdapterLuid(HANDLE hResource, LUID *pLuid) override;
	virtual HRESULT __stdcall RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie) override;
	virtual HRESULT __stdcall RegisterStereoStatusEvent(HANDLE hEvent, DWORD *pdwCookie) override;
	virtual void __stdcall UnregisterStereoStatus(DWORD dwCookie) override;
	virtual HRESULT __stdcall RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie) override;
	virtual HRESULT __stdcall RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD *pdwCookie) override;
	virtual void __stdcall UnregisterOcclusionStatus(DWORD dwCookie) override;
	virtual HRESULT __stdcall CreateSwapChainForComposition(
			IUnknown *pDevice, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput,
			IDXGISwapChain1 **ppSwapChain
	) override;
};

} // namespace DxWrappers
