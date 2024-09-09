#include "FactoryWrapper2.hpp"
#include "infra/Log.hpp"

namespace DxWrappers {

DXGIFactoryWrapper2::DXGIFactoryWrapper2(ComPtr<IDXGIFactory2> target) {
	_target = target;
	D2D1_FACTORY_OPTIONS opt = {D2D1_DEBUG_LEVEL_ERROR};
	HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &opt,
			reinterpret_cast<void **>(_D2DFactory.GetAddressOf())
	);

	if (!SUCCEEDED(hr))
		AutomataMod::log(AutomataMod::LogLevel::LOG_ERROR, "D2D1CreateFactory failed with code {}", hr);
}

DXGIFactoryWrapper2::~DXGIFactoryWrapper2() {}

void DXGIFactoryWrapper2::toggleDvdMode(bool enabled) {
	if (_currentSwapChain)
		_currentSwapChain->toggleDvdMode(enabled);
}

HRESULT __stdcall DXGIFactoryWrapper2::QueryInterface(REFIID riid, void **ppvObject) {
	if (riid == __uuidof(IDXGIFactory2) || riid == __uuidof(IDXGIFactory1) || riid == __uuidof(IDXGIFactory) ||
			riid == __uuidof(IDXGIObject) || riid == __uuidof(IUnknown)) {
		this->AddRef();
		*ppvObject = this;
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}

ULONG __stdcall DXGIFactoryWrapper2::AddRef() { return _refCounter.incrementRef(); }

ULONG __stdcall DXGIFactoryWrapper2::Release() {
	return _refCounter.decrementRef([this](ULONG refCount) {
		if (refCount == 0) {
			AutomataMod::log(AutomataMod::LogLevel::LOG_DEBUG, "DXGIFactoryWrapper2 ref count is zero. clearing.");
			_currentSwapChain = nullptr;
			_D2DFactory = nullptr;
			_target = nullptr;
		}
	});
}

HRESULT __stdcall DXGIFactoryWrapper2::SetPrivateData(REFGUID Name, UINT DataSize, const void *pData) {
	return _target->SetPrivateData(Name, DataSize, pData);
}

HRESULT __stdcall DXGIFactoryWrapper2::SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown) {
	return _target->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT __stdcall DXGIFactoryWrapper2::GetPrivateData(REFGUID Name, UINT *pDataSize, void *pData) {
	return _target->GetPrivateData(Name, pDataSize, pData);
}

HRESULT __stdcall DXGIFactoryWrapper2::GetParent(REFIID riid, void **ppParent) {
	return _target->GetParent(riid, ppParent);
}

HRESULT __stdcall DXGIFactoryWrapper2::EnumAdapters(UINT Adapter, IDXGIAdapter **ppAdapter) {
	return _target->EnumAdapters(Adapter, ppAdapter);
}

HRESULT __stdcall DXGIFactoryWrapper2::MakeWindowAssociation(HWND WindowHandle, UINT Flags) {
	return _target->MakeWindowAssociation(WindowHandle, Flags);
}

HRESULT __stdcall DXGIFactoryWrapper2::GetWindowAssociation(HWND *pWindowHandle) {
	return _target->GetWindowAssociation(pWindowHandle);
}

HRESULT __stdcall DXGIFactoryWrapper2::CreateSwapChain(
		IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain
) {
	return _target->CreateSwapChain(pDevice, pDesc, ppSwapChain);
}

HRESULT __stdcall DXGIFactoryWrapper2::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter **ppAdapter) {
	return _target->CreateSoftwareAdapter(Module, ppAdapter);
}

HRESULT __stdcall DXGIFactoryWrapper2::EnumAdapters1(UINT Adapter, IDXGIAdapter1 **ppAdapter) {
	return _target->EnumAdapters1(Adapter, ppAdapter);
}

BOOL __stdcall DXGIFactoryWrapper2::IsCurrent() { return _target->IsCurrent(); }

BOOL __stdcall DXGIFactoryWrapper2::IsWindowedStereoEnabled() { return _target->IsWindowedStereoEnabled(); }

HRESULT __stdcall DXGIFactoryWrapper2::CreateSwapChainForHwnd(
		IUnknown *pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc,
		const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc, IDXGIOutput *pRestrictToOutput,
		IDXGISwapChain1 **ppSwapChain
) {
	AutomataMod::log(AutomataMod::LogLevel::LOG_DEBUG, "CreateSwapChainForHwnd called");
	ComPtr<IDXGISwapChain1> swapChain;
	HRESULT result = _target->CreateSwapChainForHwnd(
			pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, swapChain.GetAddressOf()
	);

	if (!SUCCEEDED(result)) {
		AutomataMod::log(AutomataMod::LogLevel::LOG_ERROR, "Failed to create swapchain in CreateSwapChainForHwnd");
		*ppSwapChain = nullptr;
		return result;
	}

	_currentSwapChain = new DXGISwapChainWrapper1(pDevice, swapChain, _D2DFactory);
	_currentSwapChain->AddRef();
	*ppSwapChain = _currentSwapChain.get();
	return result;
}

HRESULT __stdcall DXGIFactoryWrapper2::CreateSwapChainForCoreWindow(
		IUnknown *pDevice, IUnknown *pWindow, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput,
		IDXGISwapChain1 **ppSwapChain
) {
	return _target->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
}

HRESULT __stdcall DXGIFactoryWrapper2::GetSharedResourceAdapterLuid(HANDLE hResource, LUID *pLuid) {
	return _target->GetSharedResourceAdapterLuid(hResource, pLuid);
}

HRESULT __stdcall DXGIFactoryWrapper2::RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie) {
	return _target->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT __stdcall DXGIFactoryWrapper2::RegisterStereoStatusEvent(HANDLE hEvent, DWORD *pdwCookie) {
	return _target->RegisterStereoStatusEvent(hEvent, pdwCookie);
}

void __stdcall DXGIFactoryWrapper2::UnregisterStereoStatus(DWORD dwCookie) {
	_target->UnregisterStereoStatus(dwCookie);
}

HRESULT __stdcall DXGIFactoryWrapper2::RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie) {
	return _target->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT __stdcall DXGIFactoryWrapper2::RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD *pdwCookie) {
	return _target->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
}

void __stdcall DXGIFactoryWrapper2::UnregisterOcclusionStatus(DWORD dwCookie) {
	_target->UnregisterOcclusionStatus(dwCookie);
}

HRESULT __stdcall DXGIFactoryWrapper2::CreateSwapChainForComposition(
		IUnknown *pDevice, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain
) {
	return _target->CreateSwapChainForComposition(pDevice, pDesc, pRestrictToOutput, ppSwapChain);
}

} // namespace DxWrappers
