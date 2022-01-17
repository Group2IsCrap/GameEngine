
#include "VertexTypes.h"

namespace Firelight::Graphics
{
	UnlitVertex::UnlitVertex() :
		m_pos(0.0f, 0.0f, 0.0f),
		m_texCoord(0.0f, 0.0f)
	{
	}

	void UnlitVertex::FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex)
	{
		m_pos.x = (float)assimpMesh->mVertices[vertexIndex].x;
		m_pos.y = (float)assimpMesh->mVertices[vertexIndex].y;
		m_pos.z = (float)assimpMesh->mVertices[vertexIndex].z;

		if (assimpMesh->mTextureCoords[0])
		{
			m_texCoord.x = (float)assimpMesh->mTextureCoords[0][vertexIndex].x;
			m_texCoord.y = (float)assimpMesh->mTextureCoords[0][vertexIndex].y;
		}
	}

	const D3D11_INPUT_ELEMENT_DESC UnlitVertex::sc_inputElements[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	SimpleLitVertex::SimpleLitVertex() :
		m_pos(0.0f, 0.0f, 0.0f),
		m_normal(0.0f, 0.0f, 0.0f),
		m_texCoord(0.0f, 0.0f)
	{
	}

	void SimpleLitVertex::FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex)
	{
		m_pos.x = (float)assimpMesh->mVertices[vertexIndex].x;
		m_pos.y = (float)assimpMesh->mVertices[vertexIndex].y;
		m_pos.z = (float)assimpMesh->mVertices[vertexIndex].z;

		m_normal.x = (float)assimpMesh->mNormals[vertexIndex].x;
		m_normal.y = (float)assimpMesh->mNormals[vertexIndex].y;
		m_normal.z = (float)assimpMesh->mNormals[vertexIndex].z;

		if (assimpMesh->mTextureCoords[0])
		{
			m_texCoord.x = (float)assimpMesh->mTextureCoords[0][vertexIndex].x;
			m_texCoord.y = (float)assimpMesh->mTextureCoords[0][vertexIndex].y;
		}
	}

	const D3D11_INPUT_ELEMENT_DESC SimpleLitVertex::sc_inputElements[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	FancyLitVertex::FancyLitVertex() :
		m_pos(0.0f, 0.0f, 0.0f),
		m_normal(0.0f, 0.0f, 0.0f),
		m_tangent(0.0f, 0.0f, 0.0f),
		m_bitangent(0.0f, 0.0f, 0.0f),
		m_texCoord(0.0f, 0.0f)
	{
	}

	void FancyLitVertex::FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex)
	{
		m_pos.x = (float)assimpMesh->mVertices[vertexIndex].x;
		m_pos.y = (float)assimpMesh->mVertices[vertexIndex].y;
		m_pos.z = (float)assimpMesh->mVertices[vertexIndex].z;

		m_normal.x = (float)assimpMesh->mNormals[vertexIndex].x;
		m_normal.y = (float)assimpMesh->mNormals[vertexIndex].y;
		m_normal.z = (float)assimpMesh->mNormals[vertexIndex].z;

		if (assimpMesh->mTextureCoords[0])
		{
			m_tangent.x = (float)assimpMesh->mTangents[vertexIndex].x;
			m_tangent.y = (float)assimpMesh->mTangents[vertexIndex].y;
			m_tangent.z = (float)assimpMesh->mTangents[vertexIndex].z;

			m_bitangent.x = (float)assimpMesh->mBitangents[vertexIndex].x;
			m_bitangent.y = (float)assimpMesh->mBitangents[vertexIndex].y;
			m_bitangent.z = (float)assimpMesh->mBitangents[vertexIndex].z;

			m_texCoord.x = (float)assimpMesh->mTextureCoords[0][vertexIndex].x;
			m_texCoord.y = (float)assimpMesh->mTextureCoords[0][vertexIndex].y;
		}
	}

	const D3D11_INPUT_ELEMENT_DESC FancyLitVertex::sc_inputElements[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BITANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}
