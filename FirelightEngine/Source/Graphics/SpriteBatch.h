#pragma once

#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
#include <d3d11.h>
#include "../Graphics/Data/Mesh.h"
#include "../Graphics/Data/VertexTypes.h"
#include "../Maths/Rect.h"
#include "../Graphics/Data/Texture.h"

namespace dx = DirectX;
//forward declare Quad
struct Quad;

namespace Firelight::Graphics
{
	struct Quad
	{
		float m_layer;//between 0 and 1

		//corners of Quad
		UnlitVertex m_topLeft;
		UnlitVertex m_topRight;
		UnlitVertex m_bottomLeft;
		UnlitVertex m_bottomRight;
	};

	class SpriteBatch
	{
		std::unordered_map <Texture*, std::vector<Quad>> m_spriteQuadsMap;
		std::vector<Mesh> m_spriteBatches;

		void NDCDraw(const Maths::Rectf& destRectNDC, const Maths::Rectf& sourceRect, const float layer, Texture* texture);
		void PixelDraw(const Maths::Rectf& destRectPixel, const Maths::Rectf& sourceRect, const float layer, Texture* texture);
		void WorldDraw(const Maths::Rectf& destRectWorld, const Maths::Rectf& sourceRect, const float layer, Texture* texture);

		void CreateBatches();
		const std::vector<Mesh>& GetBatches();
	};
}