#include "Mesh.h"

#include "../GraphicsHandler.h"

#include "Texture.h"
#include "Material.h"

namespace Firelight::Graphics
{
	void Mesh::SetTexture(Texture* texture)
	{
		m_texture = texture;
	}

	void Mesh::SetMaterial(Material* material)
	{
		m_material = material;
	}

	void Mesh::Draw(bool bindPSData) const
    {
		ID3D11DeviceContext* deviceContext = GraphicsHandler::Instance().GetDeviceContext();

		// If we have a texture, bind it to slot 0
		if (m_texture != nullptr && bindPSData)
		{
			deviceContext->PSSetShaderResources(0, 1, m_texture->GetShaderResourceView().GetAddressOf());
		}
		// If we have a material, bind it
		else if (m_material != nullptr)
		{
			m_material->Bind(bindPSData);
		}

		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOfBuffer(), m_vertexBuffer.GetStridePtr(), &offset);
		deviceContext->IASetIndexBuffer(m_indexBuffer.GetBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(m_indexBuffer.GetIndexCount(), 0, 0);
    }

	const std::string& Mesh::GetName() const
	{
		return m_name;
	}

	Texture* Mesh::GetTexture()
	{
		return m_texture;
	}

	Material* Mesh::GetMaterial()
	{
		return m_material;
	}

	const DirectX::XMMATRIX& Mesh::GetTransformMatrix() const
	{
		return m_transformMatrix;
	}
}