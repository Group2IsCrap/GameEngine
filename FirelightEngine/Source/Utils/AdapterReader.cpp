#include "AdapterReader.h"

#include <wrl/client.h>

namespace Firelight::Utils
{
	AdapterData::AdapterData(IDXGIAdapter* pAdapter) :
		m_adapter(pAdapter),
		m_description()
	{
		HRESULT hr = m_adapter->GetDesc(&m_description);
		COM_ERROR_IF_FAILED(hr, "Failed to get description for IDXGIAdapter");
	}

	AdapterReader& AdapterReader::Instance()
	{
		static AdapterReader instance;
		return instance;
	}

	std::vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (m_adapters.size() > 0)
		{
			return m_adapters;
		}

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
		COM_ERROR_FATAL_IF_FAILED(hr, "Failed to get description for IDXGIAdapter");

		IDXGIAdapter* pAdapter;
		UINT index = 0;

		while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
		{
			m_adapters.push_back(AdapterData(pAdapter));
			index++;
		}

		return m_adapters;
	}
}
