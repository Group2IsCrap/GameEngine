#pragma once

#include <vector>
#include <unordered_map>

#include "../Graphics/Data/Mesh.h"
#include "../Graphics/Data/VertexTypes.h"
#include "../Maths/Rect.h"

// Forward declarations
namespace Firelight::Graphics
{
	class Texture;
}

namespace Firelight::Graphics
{
	struct Quad
	{
		float m_depth;// Between 0 and 1

		// Corners of Quad
		UnlitVertex m_topLeft;
		UnlitVertex m_topRight;
		UnlitVertex m_bottomLeft;
		UnlitVertex m_bottomRight;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void NDCDraw(const Maths::Rectf& destRectNDC, const Maths::Rectf& sourceRect, int layer, Texture* texture);
		void PixelDraw(const Maths::Rectf& destRectPixel, const Maths::Rectf& sourceRect, int layer, Texture* texture);
		void WorldDraw(const Maths::Rectf& destRectWorld, const Maths::Rectf& sourceRect, int layer, Texture* texture);

		void CreateBatches();
		const std::vector<Mesh>& GetBatches();

	private:
		std::unordered_map <Texture*, std::vector<Quad>> m_spriteQuadsMap;
		std::vector<Mesh>                                m_spriteBatches;
	};
}
