#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

#include <Assimp/mesh.h>

#include "../../Maths/Vec2.h"
#include "../../Maths/Vec3.h"
#include "../../Maths/Vec4.h"

namespace Firelight::Graphics
{
	struct UnlitVertex
	{
		UnlitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		Maths::Vec3f m_pos;
		Maths::Vec2f m_texCoord;
		Maths::Vec4f m_colour;
	};

	struct SimpleLitVertex
	{
		SimpleLitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		Maths::Vec3f m_pos;
		Maths::Vec3f m_normal;
		Maths::Vec2f m_texCoord;
	};

	struct FancyLitVertex
	{
		FancyLitVertex();

		void FillUsingAssimpMesh(aiMesh* assimpMesh, int vertexIndex);

		Maths::Vec3f m_pos;
		Maths::Vec3f m_normal;
		Maths::Vec3f m_tangent;
		Maths::Vec3f m_bitangent;
		Maths::Vec2f m_texCoord;
	};
}
