#include "AdapterReader.h"

#include <wrl/client.h>

namespace TempEngineNamespace::Utils
{
	AdapterData::AdapterData(IDXGIAdapter* pAdapter) :
		mAdapter(pAdapter),
		mDescription()
	{
		HRESULT hr = mAdapter->GetDesc(&mDescription);
		COM_ERROR_IF_FAILED(hr, "Failed to get description for IDXGIAdapter");
	}

	AdapterReader& AdapterReader::Instance()
	{
		static AdapterReader instance;
		return instance;
	}

	std::vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (mAdapters.size() > 0)
		{
			return mAdapters;
		}

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
		COM_ERROR_FATAL_IF_FAILED(hr, "Failed to get description for IDXGIAdapter");

		IDXGIAdapter* pAdapter;
		UINT index = 0;

		while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
		{
			mAdapters.push_back(AdapterData(pAdapter));
			index++;
		}

		return mAdapters;
	}
}
