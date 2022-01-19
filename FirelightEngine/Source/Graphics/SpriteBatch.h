#pragma once

#include <vector>

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
	class SpriteBatch
	{
	private:
		struct Quad
		{
			// Corners of Quad
			UnlitVertex m_topLeft;
			UnlitVertex m_topRight;
			UnlitVertex m_bottomLeft;
			UnlitVertex m_bottomRight;

			int         m_layer;
			Texture*    m_texture;
		};

	public:
		enum class SortMode
		{
			e_None,
			e_JustTexture,
			e_BackToFrontTexture,
			e_FrontToBackTexture,
		};

	public:
		SpriteBatch();
		~SpriteBatch();

		// Set the mode that will be used to sort quads
		void SetSortMode(SortMode sortMode);

		// Draw with a dest rect in NDC
		void NDCDraw(const Maths::Rectf& destRectNDC, Texture* texture, int layer = 32, double rotation = 0.0, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));
		// Draw with a dest rect in screen pixels
		void PixelDraw(const Maths::Rectf& destRectPixel, Texture* texture, int layer = 32, double rotation = 0.0, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));
		// Draw with a dest rect in world units
		void WorldDraw(const Maths::Rectf& destRectWorld, Texture* texture, int layer = 32, double rotation = 0.0, const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f));

		void SortQuads();
		void CreateBatches();
		const std::vector<Mesh>& GetBatches();

		void DeleteQueueData();

	private:
		std::vector<Quad*> m_quadQueue;
		int                m_queueLength;

		std::vector<Mesh>  m_spriteBatches;

		SortMode           m_sortMode;
	};
}
