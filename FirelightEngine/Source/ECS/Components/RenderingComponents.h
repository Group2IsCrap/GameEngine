#pragma once

#include <string>

#include "../ECSDefines.h"

#include "../../Graphics/Data/Texture.h"
#include "../../Graphics/Data/Colour.h"

#include "../../Maths/Rect.h"

namespace Firelight::ECS
{
	/// <summary>
	/// Contains information about a camera
	/// </summary>
	struct Camera2DComponent : BaseComponent
	{
		float viewportWorldHeight = 20.0f;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			writer.Key("ViewportWorldHeight");
			writer.Double(viewportWorldHeight);
		}

		Camera2DComponent* Clone() override
		{
			Camera2DComponent* clone = new Camera2DComponent();
			clone->viewportWorldHeight = viewportWorldHeight;

			return clone;
		}
	};

	/// <summary>
	/// Contains information required to draw a sprite
	/// </summary>
	struct SpriteComponent : BaseComponent
	{
		Firelight::Graphics::Texture*     texture = nullptr;
		int                               layer = 32;

		Maths::Vec2f                      drawOffset = Maths::Vec2f(0.0f, 0.0f);
		float                             pixelsPerUnit = 100.0f;

		Firelight::Graphics::Colour::RGBA colour = Firelight::Graphics::Colours::sc_white;
		Maths::Rectf                      sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			writer.Key("Texture");
			texture->Serialize(writer);
			writer.Key("Layer");
			writer.Int(layer);
			writer.Key("DrawOffset");
			drawOffset.Serialize(writer);
			writer.Key("PixelsPerUnit");
			writer.Double(pixelsPerUnit);
			///Serialize Color
			///Serialize sourceRect
		}

		SpriteComponent* Clone() override
		{
			SpriteComponent* clone = new SpriteComponent();
			clone->texture = texture;
			clone->layer = layer;
			clone->drawOffset = drawOffset;
			clone->pixelsPerUnit = pixelsPerUnit;
			clone->colour = colour;
			clone->sourceRect = sourceRect;

			return clone;
		}
	};


	/// <summary>
	/// Contains information required to draw a NDC sprite
	/// </summary>
	struct NDCSpriteComponent : BaseComponent
	{
		Firelight::Graphics::Texture*     texture = nullptr;
		int                               layer = 32;

		Maths::Vec2f                      drawOffset = Maths::Vec2f(0.0f, 0.0f);

		Firelight::Graphics::Colour::RGBA colour = Firelight::Graphics::Colours::sc_white;
		Maths::Rectf                      sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
		Maths::Rectf                      descRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}

		NDCSpriteComponent* Clone() override
		{
			NDCSpriteComponent* clone = new NDCSpriteComponent();
			clone->texture = texture;
			clone->layer = layer;
			clone->drawOffset = drawOffset;
			clone->descRect = descRect;
			clone->colour = colour;
			clone->sourceRect = sourceRect;

			return clone;
		}
	};

	/// <summary>
	/// Contains information required to draw a Pixle sprite
	/// </summary>
	struct PixelSpriteComponent : BaseComponent
	{
		Firelight::Graphics::Texture* texture = nullptr;
		int                               layer = 32;

		Maths::Vec2f                      drawOffset = Maths::Vec2f(0.0f, 0.0f);

		Firelight::Graphics::Colour::RGBA colour = Firelight::Graphics::Colours::sc_white;
		Maths::Rectf                      sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
		bool    toDraw = true;
		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}

		PixelSpriteComponent* Clone() override
		{
			PixelSpriteComponent* clone = new PixelSpriteComponent();
			clone->texture = texture;
			clone->layer = layer;
			clone->drawOffset = drawOffset;
			clone->colour = colour;
			clone->sourceRect = sourceRect;
			clone->toDraw = toDraw;

			return clone;
		}
	};
}
