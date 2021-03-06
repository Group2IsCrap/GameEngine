#pragma once

#include <string>

#include "../ECSDefines.h"

#include "../../Graphics/Data/Texture.h"
#include "../../Graphics/Data/Colour.h"
#include "../../Graphics/Text/Text.h"

#include "../../Maths/Rect.h"
#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	/// <summary>
	/// Contains information about a camera
	/// </summary>
	struct Camera2DComponent : BaseComponent
	{
		float viewportWorldHeight = 20.0f;

		void Serialise() override
		{
			Serialiser::Serialise("ViewportWorldHeight", viewportWorldHeight);
		}

		Camera2DComponent* Clone() override
		{
			Camera2DComponent* clone = new Camera2DComponent();
			clone->viewportWorldHeight = viewportWorldHeight;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("ViewportWorldHeight", viewportWorldHeight);
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
		bool                              flipX = false;
		bool                              flipY = false;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("Texture", texture);
			Serialiser::Serialise("DrawOffset", drawOffset);
			Serialiser::Serialise("PixelsPerUnit", pixelsPerUnit);
			Serialiser::Serialise("Colour", colour);
			Serialiser::Serialise("SourceRect", sourceRect);
			Serialiser::Serialise("FlipX", flipX);
			Serialiser::Serialise("FlipY", flipY);
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
			clone->flipX = flipX;
			clone->flipY = flipY;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);
			ImGuiVariable("Texture", texture != nullptr ? "Assigned" : "Nullptr");
			ImGuiVariable("DrawOffset", drawOffset);
			ImGuiVariable("PixelsPerUnit", pixelsPerUnit);
			ImGuiVariable("Colour", std::to_string(colour.GetR()) + ", " + std::to_string(colour.GetG()) + ", " + std::to_string(colour.GetB()) + ", " + std::to_string(colour.GetA()));
			ImGuiVariable("SourceRect", sourceRect);
			ImGuiVariable("FlipX", flipX);
			ImGuiVariable("FlipY", flipY);
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
		Maths::Rectf                      destRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
		bool                              flipX = false;
		bool                              flipY = false;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("Texture", texture);
			Serialiser::Serialise("DrawOffset", drawOffset);
			Serialiser::Serialise("Colour", colour);
			Serialiser::Serialise("SourceRect", sourceRect);
			Serialiser::Serialise("DestRect", destRect);
			Serialiser::Serialise("FlipX", flipX);
			Serialiser::Serialise("FlipY", flipY);
		}

		NDCSpriteComponent* Clone() override
		{
			NDCSpriteComponent* clone = new NDCSpriteComponent();
			clone->texture = texture;
			clone->layer = layer;
			clone->drawOffset = drawOffset;
			clone->destRect = destRect;
			clone->colour = colour;
			clone->sourceRect = sourceRect;
			clone->flipX = flipX;
			clone->flipY = flipY;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);
			ImGuiVariable("Texture", texture != nullptr ? "Assigned" : "Nullptr");
			ImGuiVariable("DrawOffset", drawOffset);
			ImGuiVariable("Colour", std::to_string(colour.GetR()) + ", " + std::to_string(colour.GetG()) + ", " + std::to_string(colour.GetB()) + ", " + std::to_string(colour.GetA()));
			ImGuiVariable("SourceRect", sourceRect);
			ImGuiVariable("DestRect", destRect);
			ImGuiVariable("FlipX", flipX);
			ImGuiVariable("FlipY", flipY);
		}
	};

	/// <summary>
	/// Contains information required to draw a Pixle sprite
	/// </summary>
	struct PixelSpriteComponent : BaseComponent
	{
		Firelight::Graphics::Texture*		texture = nullptr;
		int									layer = 32;

		Maths::Vec2f						drawOffset = Maths::Vec2f(0.0f, 0.0f);

		Firelight::Graphics::Colour::RGBA	colour = Firelight::Graphics::Colours::sc_white;
		Maths::Rectf						sourceRect = Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
		bool								toDraw = true;
		bool                                flipX = false;
		bool                                flipY = false;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("Texture", texture);
			Serialiser::Serialise("DrawOffset", drawOffset);
			Serialiser::Serialise("Colour", colour);
			Serialiser::Serialise("SourceRect", sourceRect);
			Serialiser::Serialise("ToDraw", toDraw);
			Serialiser::Serialise("FlipX", flipX);
			Serialiser::Serialise("FlipY", flipY);
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
			clone->flipX = flipX;
			clone->flipY = flipY;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);   
			ImGuiVariable("Texture", texture != nullptr ? "Assigned" : "Nullptr");
			ImGuiVariable("DrawOffset", drawOffset);
			ImGuiVariable("Colour", colour);
			ImGuiVariable("To Draw", toDraw);
			ImGuiVariable("SourceRect", sourceRect);
			ImGuiVariable("FlipX", flipX);
			ImGuiVariable("FlipY", flipY);
		}
	};

	/// <summary>
	/// Contains a text object
	/// </summary>
	struct TextComponent : BaseComponent
	{
		Firelight::Graphics::Text text;
		int                       layer = 32;
		bool                      hidden = false;

		void Serialise() override
		{
			Serialiser::Serialise("Text", text);
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("Hidden", hidden);
		}

		TextComponent* Clone() override
		{
			TextComponent* clone = new TextComponent();
			clone->text = text;
			clone->layer = layer;
			clone->hidden = hidden;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Text", text.GetString());
			ImGuiVariable("Layer", layer);
			ImGuiVariable("Hidden", hidden ? "true" : "false");
		}
	};
}
