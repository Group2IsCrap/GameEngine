#pragma once

#include "../../Maths/Rect.h"

namespace Firelight::Graphics
{
	struct FontGlyph
	{
		FontGlyph(Maths::Recti charImageRect, Maths::Vec2i charOffset, unsigned int advanceX);
		FontGlyph();

		Maths::Recti m_charImageRect;
		Maths::Vec2i m_charOffset;

		unsigned int m_advanceX;
	};
}
