
#include "Mesh.h"

#include <DirectXMath.h>

#include "../GraphicsHandler.h"

namespace Firelight::Graphics
{
	template<typename VertexType>
	inline Mesh::Mesh(std::vector<VertexType>& vertices, std::vector<DWORD>& indices) :
		m_name("null"),

		m_texture(nullptr),
		m_material(nullptr),

		m_transformMatrix(DirectX::XMMatrixIdentity())
	{
		CreateBuffers<VertexType>(vertices, indices);
	}

	template<typename VertexType>
	inline Mesh::Mesh(std::vector<VertexType>& vertices, std::vector<DWORD>& indices, Texture* texture) :
		m_name("null"),

		m_texture(texture),
		m_material(nullptr),

		m_transformMatrix(DirectX::XMMatrixIdentity())
	{
		CreateBuffers<VertexType>(vertices, indices);
	}

	template<typename VertexType>
	inline Mesh::Mesh(const std::string& name, std::vector<VertexType>& vertices, std::vector<DWORD>& indices, const DirectX::XMMATRIX& transformMatrix) :
		m_name(name),

		m_texture(nullptr),
		m_material(nullptr),

		m_transformMatrix(transformMatrix)
	{
		CreateBuffers<VertexType>(vertices, indices);
	}

	template<typename VertexType>
	inline Mesh::Mesh(const std::string& name, std::vector<VertexType>& vertices, std::vector<DWORD>& indices, Material* material, const DirectX::XMMATRIX& transformMatrix) :
		m_name(name),

		m_texture(nullptr),
		m_material(material),

		m_transformMatrix(transformMatrix)
	{
		CreateBuffers<VertexType>(vertices, indices);
	}

	inline Mesh::Mesh(const Mesh& mesh) :
		m_indexBuffer(mesh.m_indexBuffer),
		m_vertexBuffer(mesh.m_vertexBuffer),
		m_texture(mesh.m_texture),
		m_material(mesh.m_material),
		m_transformMatrix(mesh.m_transformMatrix)
	{
	}

	template<typename VertexType>
	inline void Mesh::CreateBuffers(std::vector<VertexType>& vertices, std::vector<DWORD>& indices)
	{
		HRESULT hr = m_vertexBuffer.Initialize(vertices.data(), sizeof(VertexType), static_cast<UINT>(vertices.size()));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh");

		hr = m_indexBuffer.Initialize(indices.data(), static_cast<UINT>(indices.size()));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh");
	}
}
