#include"UIBase.h"
#include<dwrite.h>

namespace Firelight::UI 
{



	UIBase::UIBase()
	{
		
	}

	UIBase::~UIBase()
	{
	}

	void UIBase::HandleEvents(void* data)
	{

		//get mouse input
		

		//get controller input 




	}

	TextRender::TextRender(const std::string& filename, Graphics::Colour chroma):
		chroma(chroma)
	{
		SpriteTex.Initialise(filename);
		glyphWidth = SpriteTex.GetWidth() / nColumns;
		glyphHeight = SpriteTex.GetHeight() / nRows;
	}

	TextRender::~TextRender()
	{
	}

	void TextRender::DrawTextFromSpriteSheet(const std::string& text, const Maths::Vec2f pos, Graphics::Colour colour)
	{
		// curPos is the pos that we are drawing to on the screen
		auto curPos = pos;
		for (auto c : text)
		{
			// on a newline character, reset x position and move down by 1 glyph height
			if (c == '\n')
			{
				// carriage return
				curPos.x = pos.x;
				// line feed
				curPos.y += glyphHeight;
				// we don't want to advance the character position right for a newline
				continue;
			}
			// only draw characters that are on the font sheet
			// start at firstChar + 1 because might as well skip ' ' as well
			else if (c >= firstChar + 1 && c <= lastChar)
			{

				//sprite batch draw 
			/*	void NDCDraw(
					Maths::Rectf(curPos.x, curPos.y,100,100),
					SpriteTex,
					0,
					0.0,
					Graphics::Colours::sc_white,
					MapGlyphRect(c));*/
			}
			// advance screen pos for next character
			curPos.x += glyphWidth;
		}

	}

		Maths::Rectf Firelight::UI::TextRender::MapGlyphRect(char c) const
		{
			assert(c >= firstChar && c <= lastChar);
			// font sheet glyphs start at ' ', calculate index into sheet
			const int glyphIndex = c - ' ';
			// map 1d glyphIndex to 2D coordinates
			const int yGlyph = glyphIndex / nColumns;
			const int xGlyph = glyphIndex % nColumns;
			// convert the sheet grid coords to pixel coords in sheet
			return Maths::Rectf(
				xGlyph * glyphWidth, yGlyph * glyphHeight,
				glyphWidth, glyphHeight
			);
		}
}