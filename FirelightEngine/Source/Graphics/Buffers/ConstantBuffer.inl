#include "ConstantBuffer.h"

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	template<class T>
	inline ConstantBuffer<T>::ConstantBuffer() :
		m_buffer(nullptr),
		m_deviceContext(nullptr)

	{
	}

	template<class T>
	inline HRESULT ConstantBuffer<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (m_buffer.Get() != nullptr)
		{
			m_buffer.Reset();
		}

		m_deviceContext = deviceContext;

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));

		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufferDesc.StructureByteStride = 0;

		return device->CreateBuffer(&constantBufferDesc, 0, m_buffer.GetAddressOf());
	}

	template<class T>
	inline ID3D11Buffer* ConstantBuffer<T>::Get() const
	{
		return m_buffer.Get();
	}

	template<class T>
	inline ID3D11Buffer* const* ConstantBuffer<T>::GetAddressOf() const
	{
		return m_buffer.GetAddressOf();
	}

	template<class T>
	inline bool ConstantBuffer<T>::MapToGPU()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = m_deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to map constant buffer to GPU", false);

		CopyMemory(mappedResource.pData, &m_data, sizeof(T));
		m_deviceContext->Unmap(m_buffer.Get(), 0);

		return true;
	}
}
