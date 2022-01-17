#pragma once

#include <unordered_map>

#include <DirectXMath.h>

#include "Mesh.h"

#include "../Buffers/ConstantBuffer.h"
#include "../Buffers/ConstantBufferTypes.h"

namespace Firelight::Graphics
{
	class Model
	{
	public:
		Model();

		template<typename VertexType>
		bool Initialise(const std::string& filePath);

		void Draw(const DirectX::XMMATRIX& modelMatrix, ConstantBuffer<PerObjectCB>* perObjectCB, bool bindPSData = true) const;
		void DrawRaw(bool bindPSData = true);

		const std::string&       GetFilePath() const;
		const std::string&       GetDirectory() const;

		const std::vector<Mesh>& GetMeshes() const;

	private:
		template<typename VertexType>
		void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
		template<typename VertexType>
		Mesh ProcessMesh(aiMesh* mesh, const DirectX::XMMATRIX& transformMatrix);

		void GetMeshMaterialsFromFile(const std::string& filePath, std::unordered_map<std::string, Material*>& meshMaterials);
		void GenerateDefaultMaterialFile(const std::string& filePath) const;

		void AssignMaterialsToMeshes(std::unordered_map<std::string, Material*>& meshMaterials);

	private:
		std::string       m_filePath;
		std::string       m_directory;

		std::vector<Mesh> m_meshes;
	};
}

#include "Model.inl"
