#pragma once

#include "Model.h"

namespace Firelight::Graphics
{
	template<typename VertexType>
	inline void Model::ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix)
	{
		DirectX::XMMATRIX nodeTransformationMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			m_meshes.push_back(ProcessMesh<VertexType>(mesh, nodeTransformationMatrix));
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode<VertexType>(node->mChildren[i], scene, nodeTransformationMatrix);
		}
	}

	template<typename VertexType>
	Mesh Model::ProcessMesh(aiMesh* mesh, const DirectX::XMMATRIX& transformMatrix)
	{
		std::vector<VertexType> meshVertices;
		std::vector<DWORD> indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{

			Vertex vertex;
			vertex.FillUsingAssimpMesh(mesh, i);

			meshVertices.push_back(vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh(mesh->mName.C_Str(), meshVertices, indices, transformMatrix);
	}

	template<typename VertexType>
	inline bool Model::Initialize(const std::string& filePath)
	{
		m_filePath = filePath;
		m_directory = Utils::StringHelpers::GetDirectoryFromPath(filePath);

		// Parse model file using assimp
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
		ASSERT_RETURN(pScene != nullptr, "Assimp failed to parse model file at path: " + filePath, false);

		// Extract model data from assimp scene object
		ProcessNode<VertexType>(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
		ASSERT_RETURN(!m_meshes.empty(), "No meshes were loaded from model at path: " + filePath, false);

		// Attempt to load linked materials from '.mtls' file and assign them to the loaded meshes
		std::string meshMaterialFilePath = Utils::StringHelpers::ChangeFileExtension(filePath, "mtls");
		std::unordered_map<std::string, Material*> meshMaterials;
		GetMeshMaterialsFromFile(meshMaterialFilePath, meshMaterials);

		// Assign the loaded mesh materials to the meshes
		AssignMaterialsToMeshes(meshMaterials);

		return true;
	}
}
