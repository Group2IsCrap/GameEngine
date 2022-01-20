#pragma once

#include "../../Maths/Vec4.h"

namespace Firelight::Graphics
{
	typedef unsigned char BYTE;

	class Colour
	{
	public:
		Colour();
		Colour(BYTE assignAllVal);
		Colour(unsigned int uintVal);
		Colour(BYTE r, BYTE g, BYTE b);
		Colour(BYTE r, BYTE g, BYTE b, BYTE a);
		Colour(const Colour& src);

		Colour&        operator=(const Colour& src);
		bool           operator==(const Colour& rhs) const;
		bool           operator!=(const Colour& rhs) const;

		constexpr BYTE GetR() const;
		void SetR(BYTE r);

		constexpr BYTE GetG() const;
		void SetG(BYTE g);

		constexpr BYTE GetB() const;
		void SetB(BYTE b);

		constexpr BYTE GetA() const;
		void SetA(BYTE a);

		unsigned int GetUnsignedInt() const;
		Maths::Vec4f GetAsNormalisedVec4() const;

	private:
		union
		{
			BYTE         m_rgba[4];
			unsigned int m_colour;
		};
	};

	namespace Colours
	{
		static const Colour sc_white(255, 255, 255);
		static const Colour sc_black(0, 0, 0);

		static const Colour sc_unhandledTextureColour(255, 0, 255);

		static const Colour sc_defaultAlbedo(170, 170, 170);
		static const Colour sc_defaultRoughness(170, 170, 170);
		static const Colour sc_defaultNormal(128, 128, 255);
		static const Colour sc_defaultMetallic(0, 0, 0);
		static const Colour sc_defaultEmission(0, 0, 0);
		static const Colour sc_defaultDepth(0, 0, 0);
	}
}