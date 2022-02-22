#pragma once

#include "../../Maths/Vec3.h"
#include "../../Maths/Vec4.h"

namespace Firelight::Graphics
{
	namespace Colour
	{
		typedef unsigned char BYTE;

		// Forward declare
		class HSLA;

		class RGBA
		{
		public:
			RGBA();
			RGBA(BYTE assignAllVal);
			RGBA(unsigned int uintVal);
			RGBA(BYTE r, BYTE g, BYTE b);
			RGBA(BYTE r, BYTE g, BYTE b, BYTE a);

			RGBA(const RGBA& src);

			RGBA& operator=(const RGBA& src);
			bool           operator==(const RGBA& rhs) const;
			bool           operator!=(const RGBA& rhs) const;

			BYTE         GetR() const;
			void         SetR(BYTE r);

			BYTE         GetG() const;
			void         SetG(BYTE g);

			BYTE         GetB() const;
			void         SetB(BYTE b);

			BYTE         GetA() const;
			void         SetA(BYTE a);

			unsigned int GetUnsignedInt() const;

			Maths::Vec3f GetAsNormalisedVec3() const;
			Maths::Vec4f GetAsNormalisedVec4() const;
			
			static RGBA  GetRandom();

			static RGBA MakeFromHSLA(const HSLA& hsl);

		private:
			union
			{
				BYTE         m_rgba[4];
				unsigned int m_colour;
			};
		};

		class HSLA
		{
		public:
			HSLA();
			HSLA(BYTE hue, BYTE saturation, BYTE lightness);
			HSLA(BYTE hue, BYTE saturation, BYTE lightness, BYTE alpha);

			HSLA(const HSLA& src);

			BYTE  GetH() const;
			float GetNormH() const;
			void  SetH(BYTE hue);

			BYTE  GetS() const;
			float GetNormS() const;
			void  SetS(BYTE saturation);

			BYTE  GetL() const;
			float GetNormL() const;
			void  SetL(BYTE lightness);

			BYTE  GetA() const;
			void  SetA(BYTE a);

			static HSLA GetRandom();
			static HSLA GetRandomHueAndLightness(BYTE saturation);
			static HSLA GetRandomHueAndSaturation(BYTE lightness);
			static HSLA GetRandomHue(BYTE saturation, BYTE lightness);

			static HSLA MakeFromRGBA(const RGBA& rgba);

		private:
			BYTE m_hue;
			BYTE m_saturation;
			BYTE m_lightness;
			BYTE m_alpha;
		};
	}

	namespace Colours
	{
		static const Colour::RGBA sc_white(255, 255, 255);
		static const Colour::RGBA sc_black(0, 0, 0);

		static const Colour::RGBA sc_unhandledTextureColour(255, 0, 255);

		static const Colour::RGBA sc_defaultAlbedo(170, 170, 170);
		static const Colour::RGBA sc_defaultRoughness(170, 170, 170);
		static const Colour::RGBA sc_defaultNormal(128, 128, 255);
		static const Colour::RGBA sc_defaultMetallic(0, 0, 0);
		static const Colour::RGBA sc_defaultEmission(0, 0, 0);
		static const Colour::RGBA sc_defaultDepth(0, 0, 0);
	}
}
