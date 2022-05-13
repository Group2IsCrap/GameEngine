#include "FontGlyph.h"

namespace Firelight::Graphics
{
	FontGlyph::FontGlyph(Maths::Recti charImageRect, Maths::Vec2i charOffset, unsigned int advanceX) :
		m_charImageRect(charImageRect),

		m_charOffset(charOffset),

		m_advanceX(advanceX)
	{
	}

	FontGlyph::FontGlyph()
	{
	}
}
