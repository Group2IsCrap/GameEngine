
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

		ID3D11Buffer*        Get() const;
		ID3D11Buffer* const* GetAddressOf() const;

		UINT                 GetIndexCount() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		UINT                                 m_indexCount;
	};
}
