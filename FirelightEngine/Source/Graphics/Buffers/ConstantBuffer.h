#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace Firelight::Graphics
{
	template<class T>
	class ConstantBuffer
	{
	
	public:
		ConstantBuffer();

		HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		ID3D11Buffer*        Get() const;
		ID3D11Buffer* const* GetAddressOf() const;

		bool MapToGPU();

	private:
		ConstantBuffer(const ConstantBuffer<T>& rhs);

	public:
		T m_data;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		ID3D11DeviceContext*                 m_deviceContext;

	};
}

#include "ConstantBuffer.inl"
