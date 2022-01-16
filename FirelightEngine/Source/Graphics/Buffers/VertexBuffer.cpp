#include "VertexBuffer.h"

#include "../GraphicsHandler.h"

namespace Firelight::Graphics
{
    VertexBuffer::VertexBuffer() :
		m_buffer(nullptr),
		m_stride(0),
		m_vertexCount(0)
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& rhs) :
		m_buffer(rhs.m_buffer),
		m_vertexCount(rhs.m_vertexCount),
		m_stride(rhs.m_stride)
	{
	}

	VertexBuffer& VertexBuffer::operator=(const VertexBuffer& a)
	{
		m_buffer = a.m_buffer;
		m_vertexCount = a.m_vertexCount;
		m_stride = a.m_stride;
		return *this;
	}

	HRESULT VertexBuffer::Initialize(const void* data, UINT stride, UINT vertexCount)
	{
		if (m_buffer.Get() != nullptr)
		{
			m_buffer.Reset();
		}

		m_vertexCount = vertexCount;
		m_stride = stride;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = m_stride * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

		vertexBufferData.pSysMem = data;

		return GraphicsHandler::Instance().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_buffer.GetAddressOf());
	}

	ID3D11Buffer* VertexBuffer::Get() const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* VertexBuffer::GetAddressOf() const
	{
		return m_buffer.GetAddressOf();
	}

	const UINT VertexBuffer::GetStride() const
	{
		return m_stride;
	}

	const UINT* VertexBuffer::GetStridePtr() const
	{
		return &m_stride;
	}

	UINT VertexBuffer::GetVertexCount() const
	{
		return m_vertexCount;
	}
}
