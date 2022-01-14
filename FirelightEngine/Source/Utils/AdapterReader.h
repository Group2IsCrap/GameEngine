#pragma once

// Retrieves and stores information about the available graphics adapters

#include <d3d11.h>
#include <dxgi.h>
#include <vector>

#include "ErrorManager.h"

namespace Firelight::Utils
{
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* pAdapter);

		IDXGIAdapter*     m_adapter;
		DXGI_ADAPTER_DESC m_description;
	};

	class AdapterReader
	{
	public:
		static AdapterReader& Instance();

		std::vector<AdapterData> GetAdapters();

	private:
		std::vector<AdapterData> m_adapters;
	};
}