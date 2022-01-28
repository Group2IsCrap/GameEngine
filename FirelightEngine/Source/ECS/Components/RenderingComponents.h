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
	};
}
