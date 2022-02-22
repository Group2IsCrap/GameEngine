#include "Font.h"

#include <fstream>

#include "../AssetManager.h"
#include "../../Utils/ErrorManager.h"

#include "../../Maths/Rect.h"

#include "../../Engine.h"

namespace Firelight::Graphics
{
    Font::Font()
    {
    }

    Font::~Font()
    {
    }

    void Font::LoadFont(std::string path)
    {
		std::string texturePath = path + ".png";
		m_texture = AssetManager::Instance().GetTexture(texturePath.c_str());

		std::string fontFilePath = "Assets/" + path + ".fnt";
		std::ifstream fontFile(fontFilePath.c_str());
		std::string buffer;

		auto loadNextValue = [&]()
		{
			char currentChar = ' ';
			bool insideValue = false;
			buffer = "";

			while (!fontFile.eof())
			{
				currentChar = (char)fontFile.get();

				if (insideValue && currentChar == ' ')
				{
					return;
				}

				if (insideValue)
				{
					buffer += currentChar;
				}

				if (currentChar == '=')
				{
					insideValue = true;
				}
			}
		};

		if (fontFile.is_open())
		{
			// Loading the wanky filetype exported by hiero

			// Info
			// Discard typeface name
			loadNextValue();
			// Discard size
			loadNextValue();
			// Discard bold
			loadNextValue();
			// Discard italic
			loadNextValue();
			// Discard charset
			loadNextValue();
			// Discard unicode
			loadNextValue();
			// Discard stretchH
			loadNextValue();
			// Discard smooth
			loadNextValue();
			// Discard aa
			loadNextValue();

			// Load padding
			loadNextValue();
			auto intValues = Utils::StringHelpers::IntVectorFromCommaSeparatedValues(buffer);
			for (int valueIndex = 0; valueIndex < 4; ++valueIndex)
			{
				m_padding[valueIndex] = intValues[valueIndex];
			}
			
			// Load spacing
			loadNextValue();
			intValues = Utils::StringHelpers::IntVectorFromCommaSeparatedValues(buffer);
			for (int valueIndex = 0; valueIndex < 2; ++valueIndex)
			{
				m_spacing[valueIndex] = intValues[valueIndex];
			}

			// Common
			// Load lineheight
			loadNextValue();
			m_lineHeight = (float)std::atoi(buffer.c_str());

			// Load base
			loadNextValue();
			m_baseLine = (float)std::atoi(buffer.c_str());

			// Load expected image width and height
			loadNextValue();
			m_expectedImageSize.x = std::atoi(buffer.c_str());
			loadNextValue();
			m_expectedImageSize.y = std::atoi(buffer.c_str());

			// Discard pages
			loadNextValue();
			// Discard packed
			loadNextValue();

			// Page
			// Discard page id
			loadNextValue();
			// Discard file path
			loadNextValue();

			// Chars
			// Load count
			loadNextValue();
			m_charCount = std::atoi(buffer.c_str());

			unsigned int charId, advanceX;
			Maths::Recti charImageRect;
			Maths::Vec2i charOffset;

			// Load character data
			for (int charIndex = 0; charIndex < m_charCount; ++charIndex)
			{
				// Load id
				loadNextValue();
				charId = std::atoi(buffer.c_str());

				// Load position
				loadNextValue();
				charImageRect.x = std::atoi(buffer.c_str());
				loadNextValue();
				charImageRect.y = std::atoi(buffer.c_str());

				// Load width and height
				loadNextValue();
				charImageRect.w = std::atoi(buffer.c_str());
				loadNextValue();
				charImageRect.h = std::atoi(buffer.c_str());

				// Load x and y offset
				loadNextValue();
				charOffset.x = std::atoi(buffer.c_str());
				loadNextValue();
				charOffset.y = std::atoi(buffer.c_str());

				// Load x advance
				loadNextValue();
				advanceX = std::atoi(buffer.c_str());

				// Discard page
				loadNextValue();

				// Discard channel
				loadNextValue();

				FontGlyph glyph(charImageRect, charOffset, advanceX);
				m_glyphs.insert({ (char)charId, glyph });
			}
		}
    }

    void Font::RenderText(std::string text, std::vector<UnlitVertex>& vertices, std::vector<DWORD>& indices, Maths::Vec2f& outBaseTextExtents, float spacingMod)
    {
		Maths::Vec2f minPos(FLT_MAX, FLT_MAX);
		Maths::Vec2f maxPos(-FLT_MAX, -FLT_MAX);

		float xOffset = 0.0f;

		for (int i = 0; i < text.length(); i++)
		{
			FontGlyph glyph = m_glyphs[text.at(i)];

			UnlitVertex topLeft;
			UnlitVertex topRight;
			UnlitVertex botLeft;
			UnlitVertex botRight;

			topLeft.m_pos = Maths::Vec3f(xOffset + (float)glyph.m_charOffset.x, (float)-glyph.m_charOffset.y, 0.0f);
			topLeft.m_texCoord = Maths::Vec2f((float)glyph.m_charImageRect.x / (float)(m_expectedImageSize.x),
				(float)(glyph.m_charImageRect.y) / (float)(m_expectedImageSize.y));

			topRight.m_pos = Maths::Vec3f(xOffset + (float)(glyph.m_charOffset.x + glyph.m_charImageRect.w), (float)-glyph.m_charOffset.y, 0.0f);
			topRight.m_texCoord = Maths::Vec2f((float)(glyph.m_charImageRect.x + glyph.m_charImageRect.w) / (float)(m_expectedImageSize.x),
				(float)(glyph.m_charImageRect.y) / (float)(m_expectedImageSize.y));

			botLeft.m_pos = Maths::Vec3f(xOffset + (float)glyph.m_charOffset.x, (float)(-glyph.m_charOffset.y - glyph.m_charImageRect.h), 0.0f);
			botLeft.m_texCoord = Maths::Vec2f((float)glyph.m_charImageRect.x / (float)m_expectedImageSize.x,
				(float)(glyph.m_charImageRect.y + glyph.m_charImageRect.h) / (float)(m_expectedImageSize.y));

			botRight.m_pos = Maths::Vec3f(xOffset + (float)(glyph.m_charOffset.x + glyph.m_charImageRect.w), (float)(-glyph.m_charOffset.y - glyph.m_charImageRect.h), 0.0f);
			botRight.m_texCoord = Maths::Vec2f((float)(glyph.m_charImageRect.x + glyph.m_charImageRect.w) / (float)(m_expectedImageSize.x),
				(float)(glyph.m_charImageRect.y + glyph.m_charImageRect.h) / (float)m_expectedImageSize.y);

			int numVertices = vertices.size();

			vertices.push_back(topLeft);
			vertices.push_back(topRight);
			vertices.push_back(botLeft);
			vertices.push_back(botRight);

			indices.push_back(numVertices);
			indices.push_back(numVertices + 1);
			indices.push_back(numVertices + 2);

			indices.push_back(numVertices + 2);
			indices.push_back(numVertices + 1);
			indices.push_back(numVertices + 3);

			xOffset += glyph.m_advanceX * spacingMod;

			Font::UpdateMinAndMaxPositions(minPos, maxPos, topLeft.m_pos);
			Font::UpdateMinAndMaxPositions(minPos, maxPos, topRight.m_pos);
			Font::UpdateMinAndMaxPositions(minPos, maxPos, botLeft.m_pos);
			Font::UpdateMinAndMaxPositions(minPos, maxPos, botRight.m_pos);
		}

		outBaseTextExtents = Maths::Vec2f(maxPos.x - minPos.x, maxPos.y - minPos.y);

		// Convert from pixel space to NDC
		int numVerts = static_cast<int>(vertices.size());
		for (int vertexIndex = 0; vertexIndex < numVerts; ++vertexIndex)
		{
			Maths::Vec3f& vertexPos = vertices[vertexIndex].m_pos;

			// Move to centre
			vertexPos.x -= outBaseTextExtents.x * 0.5f;
			vertexPos.y += m_lineHeight * 0.5f;

			// Normalize
			vertexPos.x /= outBaseTextExtents.x * 0.5f;
			vertexPos.y /= m_lineHeight * 0.5f;
		}

		// Return size 1 text extents
		outBaseTextExtents /= m_lineHeight;
    }

    void Font::UpdateMinAndMaxPositions(Maths::Vec2f& minPos, Maths::Vec2f& maxPos, const Maths::Vec3f& checkPos)
    {
		if (checkPos.x > maxPos.x)
		{
			maxPos.x = checkPos.x;
		}
		if (checkPos.y > maxPos.y)
		{
			maxPos.y = checkPos.y;
		}
		if (checkPos.x < minPos.x)
		{
			minPos.x = checkPos.x;
		}
		if (checkPos.y < minPos.y)
		{
			minPos.y = checkPos.y;
		}
    }

    Texture* Font::GetTexture()
    {
        return m_texture;
    }
}
