
//Stores an ID3D11Buffer for some vertices and their meta-data, automatically sets up buffer description and buffer subresource data

#pragma once

#include <memory>

#include <d3d11.h>
#include <wrl/client.h>

namespace Firelight::Graphics
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer& operator=(const VertexBuffer& a);

		HRESULT Initialize(const void* data, UINT stride, UINT vertexCount);

		ID3D11Buffer*        GetBuffer() const;
		ID3D11Buffer* const* GetAddressOfBuffer() const;

		const UINT  GetStride() const;
		const UINT* GetStridePtr() const;

		UINT GetVertexCount() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		UINT                                 m_stride;
		UINT                                 m_vertexCount;
	};
}
