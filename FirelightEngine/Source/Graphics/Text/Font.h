#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Data/Texture.h"
#include "../Data/VertexTypes.h"

#include "../../Maths/Vec2.h"
#include "../../Maths/Vec4.h"

#include "FontGlyph.h"

namespace Firelight::Graphics
{
	class Font
	{
	public:
		Font();
		~Font();

		void        LoadFont(std::string name);
		void        RenderText(std::string text, std::vector<UnlitVertex>& vertices, std::vector<DWORD>& indices, Maths::Vec2f& outBaseTextExtents, float spacingMod);
		static void UpdateMinAndMaxPositions(Maths::Vec2f& minPos, Maths::Vec2f& maxPos, const Maths::Vec3f& checkPos);

		Texture*    GetTexture();

	private:
		Maths::Vec4i m_padding;
		Maths::Vec2i m_spacing;

		float m_lineHeight;
		float m_baseLine;

		Maths::Vec2i m_expectedImageSize;
		
		int m_charCount;

		std::map<char, FontGlyph> m_glyphs;

		Texture* m_texture;
	};
}
