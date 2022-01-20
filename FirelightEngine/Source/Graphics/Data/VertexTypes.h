#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

#include <Assimp/mesh.h>

namespace Firelight::Graphics
{
	struct UnlitVertex
	{
		UnlitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT2 m_texCoord;
	};

	struct SimpleLitVertex
	{
		SimpleLitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_normal;
		DirectX::XMFLOAT2 m_texCoord;
	};

	struct FancyLitVertex
	{
		FancyLitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_normal;
		DirectX::XMFLOAT3 m_tangent;
		DirectX::XMFLOAT3 m_bitangent;
		DirectX::XMFLOAT2 m_texCoord;
	};
}
