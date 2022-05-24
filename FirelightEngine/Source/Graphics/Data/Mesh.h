#pragma once

#include <vector>
#include <string>

#include <DirectXMath.h>

#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>

#include "../Buffers/VertexBuffer.h"
#include "../Buffers/IndexBuffer.h"

// Forward declarations
namespace Firelight::Graphics
{
	class Texture;
	class Material;
}

namespace Firelight::Graphics
{
	class Mesh
	{
	public:
		template<typename VertexType>
		Mesh(std::vector<VertexType>& vertices, std::vector<DWORD>& indices);
		template<typename VertexType>
		Mesh(std::vector<VertexType>& vertices, std::vector<DWORD>& indices, Texture* texture);
		template<typename VertexType>
		Mesh(const std::string& name, std::vector<VertexType>& vertices, std::vector<DWORD>& indices, Texture* texture);
		template<typename VertexType>
		Mesh(const std::string& name, std::vector<VertexType>& vertices, std::vector<DWORD>& indices, const DirectX::XMMATRIX& transformMatrix);
		template<typename VertexType>
		Mesh(const std::string& name, std::vector<VertexType>& vertices, std::vector<DWORD>& indices, Material* material, const DirectX::XMMATRIX& transformMatrix);

		Mesh(const Mesh& mesh);

		template<typename VertexType>
		void CreateBuffers(std::vector<VertexType>& vertices, std::vector<DWORD>& indices);

		void SetTexture(Texture* material);
		void SetMaterial(Material* material);

		void                     Draw(bool bindPSData = true) const;

		const std::string&       GetName() const;
		
		Texture*                 GetTexture();
		Material*                GetMaterial();

		const DirectX::XMMATRIX& GetTransformMatrix() const;

		
	private:
		std::string  m_name;

		VertexBuffer m_vertexBuffer;
		IndexBuffer  m_indexBuffer;

		Texture*  m_texture;
		Material* m_material;

		DirectX::XMMATRIX m_transformMatrix;
	};
}

#include "Mesh.inl"
