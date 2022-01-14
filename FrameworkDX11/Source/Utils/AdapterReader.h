#pragma once

// Retrieves and stores information about the available graphics adapters

#include <d3d11.h>
#include <dxgi.h>
#include <vector>

#include "ErrorManager.h"

namespace TempEngineNamespace::Utils
{
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* pAdapter);

		IDXGIAdapter*     mAdapter;
		DXGI_ADAPTER_DESC mDescription;
	};

	class AdapterReader
	{
	public:
		static AdapterReader& Instance();

		std::vector<AdapterData> GetAdapters();

	private:
		std::vector<AdapterData> mAdapters;
	};
}