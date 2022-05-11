#pragma once

#include <vector>

#include "Data/Mesh.h"
#include "Data/VertexTypes.h"
#include "Data/Colour.h"

#include "Text/Text.h"

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
		struct LayerableEntity
		{
			LayerableEntity()
			{
			}

			virtual ~LayerableEntity()
			{
			}

			int m_layer = 0;
		};

		struct QuadEntity : public LayerableEntity
		{
			// Corners of Quad
			UnlitColourVertex m_topLeft;
			UnlitColourVertex m_topRight;
			UnlitColourVertex m_bottomLeft;
			UnlitColourVertex m_bottomRight;

			Texture* m_texture = nullptr;
		};

		struct TextEntity : public LayerableEntity
		{
			Text* m_text = nullptr;
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
		void NDCDraw(const Maths::Rectf& destRectNDC,
			Texture* texture = nullptr,
			int layer = 32,
			double rotation = 0.0,
			const Colour::RGBA& colour = Colours::sc_white,
			const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f),
			bool flipX = false, bool flipY = false
		);

		// Draw with a dest rect in screen pixels
		void PixelDraw(const Maths::Rectf& destRectPixel,
			Texture* texture = nullptr,
			int layer = 32,
			double rotation = 0.0,
			const Colour::RGBA& colour = Colours::sc_white,
			const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f),
			bool flipX = false, bool flipY = false
		);

		// Draw with a dest rect in world units
		void WorldDraw(
			const Maths::Rectf& destRectWorld,
			Texture* texture = nullptr,
			int layer = 32,
			double rotation = 0.0,
			const Colour::RGBA& colour = Colours::sc_white,
			const Maths::Rectf& sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f),
			bool flipX = false, bool flipY = false
		);

		void DrawTextStandard(Text* text, int layer = 32);

		void SortQuads();
		void CreateAndDrawBatches(bool bindPSData = true);

		void DeleteQueueData();

	private:
		std::vector<LayerableEntity*> m_entityQueue;

		SortMode           m_sortMode;
	};
}
