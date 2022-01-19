#include "Model.h"

#include <fstream>

#include "../AssetManager.h"

#include "../../Utils/StringHelpers.h"

namespace Firelight::Graphics
{
	Model::Model() :
		m_filePath(""),
		m_directory(""),

		m_meshes()
	{
	}

	void Model::Draw(const DirectX::XMMATRIX& modelMatrix, ConstantBuffer<PerObjectCB>* perObjectCB, bool bindPSData) const
	{
		for (int i = 0; i < m_meshes.size(); i++)
		{
			perObjectCB->m_data.m_modelMatrix = m_meshes[i].GetTransformMatrix() * modelMatrix;
			perObjectCB->MapToGPU();

			m_meshes[i].Draw(bindPSData);
		}
	}

	void Model::DrawRaw(bool bindPSData)
	{
		for (int i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].Draw(bindPSData);
		}
	}
	
	const std::string& Model::GetFilePath() const
	{
		return m_filePath;
	}

	const std::string& Model::GetDirectory() const
	{
		return m_directory;
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return m_meshes;
	}

	void Model::GetMeshMaterialsFromFile(const std::string& filePath, std::unordered_map<std::string, Material*>& meshMaterials)
	{
		std::ifstream materialsFile(filePath);

		if (materialsFile)
		{
			while (!materialsFile.eof())
			{
				std::string meshName;
				materialsFile >> meshName;

				std::string meshMaterialName;
				materialsFile >> meshMaterialName;

				meshMaterials.insert(std::make_pair(meshName, AssetManager::Instance().GetMaterial(meshMaterialName)));
			}

			materialsFile.close();
			return;
		}

		std::string errorMessage = "Mesh materials file not found at location: " + filePath + "\nWould you like to auto-generate one?";
		int buttonPress = MessageBoxW(NULL, Utils::StringHelpers::StringToWide(errorMessage).c_str(), L"No Material File Found", MB_YESNO);

		// Create a material file if desired
		if (buttonPress == IDYES)
		{
			GenerateDefaultMaterialFile(filePath);
		}

		// Assign default materials
		for (int meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
		{
			meshMaterials.insert(std::make_pair(m_meshes[meshIndex].GetName(), AssetManager::Instance().GetDefaultMaterial()));
		}
	}

	void Model::AssignMaterialsToMeshes(std::unordered_map<std::string, Material*>& meshMaterials)
	{
		for (int meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
		{
			// If mesh has a material, set it
			const auto& itr = meshMaterials.find(m_meshes[meshIndex].GetName());
			if (itr != meshMaterials.end())
			{
				m_meshes[meshIndex].SetMaterial(itr->second);
			}
		}
	}

	void Model::GenerateDefaultMaterialFile(const std::string& filePath) const
	{
		std::ofstream materialsFile(filePath);

		if (materialsFile)
		{
			for (int meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
			{
				materialsFile << m_meshes[meshIndex].GetName().c_str() << " " << "$ENGINE/Materials/default\n";
			}

			materialsFile.close();
			return;
		}

		ERROR_STANDARD("Couldn't open file to generate materials file for path: " + filePath);
	}
}
