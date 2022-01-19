
#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace Firelight::Graphics
{
	class IndexBuffer
	{
	public:
		IndexBuffer();

		HRESULT              Initialize(DWORD* data, UINT indexCount);

		ID3D11Buffer*        GetBuffer() const;
		ID3D11Buffer* const* GetAddressOfBuffer() const;

		UINT                 GetIndexCount() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		UINT                                 m_indexCount;
	};
}
