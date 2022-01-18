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

		// Draw with a dest rect in NDC
		void NDCDraw(const Maths::Rectf& destRectNDC, Texture* texture, int layer = 32, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));
		// Draw with a dest rect in screen pixels
		void PixelDraw(const Maths::Rectf& destRectPixel, Texture* texture, int layer = 32, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));
		// Draw with a dest rect in world units
		void WorldDraw(const Maths::Rectf& destRectWorld, Texture* texture, int layer = 32, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));

		void CreateBatches();
		const std::vector<Mesh>& GetBatches();

	private:
		std::unordered_map <Texture*, std::vector<Quad>> m_spriteQuadsMap;
		std::vector<Mesh>                                m_spriteBatches;
	};
}
