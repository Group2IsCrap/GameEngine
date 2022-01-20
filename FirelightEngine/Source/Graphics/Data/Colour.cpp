#pragma once

#include "Colour.h"

#include "../../Maths/Random.h"

namespace Firelight::Graphics::Colour
{
	RGBA::RGBA() :
		m_colour(0)
	{
	}

	RGBA::RGBA(BYTE assignAllVal) :
		RGBA(assignAllVal, assignAllVal, assignAllVal)
	{
	}

	RGBA::RGBA(unsigned int numVal) :
		m_colour(numVal)
	{
	}

	RGBA::RGBA(BYTE r, BYTE g, BYTE b) :
		RGBA(r, g, b, 255)
	{
	}

	RGBA::RGBA(BYTE r, BYTE g, BYTE b, BYTE a) :
		m_rgba{r, g, b, a}
	{
	}

	RGBA::RGBA(const RGBA& src) :
		m_colour(src.m_colour)
	{
	}

	RGBA& RGBA::operator=(const RGBA& src)
	{
		m_colour = src.m_colour;
		return *this;
	}

	bool RGBA::operator==(const RGBA& rhs) const
	{
		return (m_colour == rhs.m_colour);
	}

	bool RGBA::operator!=(const RGBA& rhs) const
	{
		return !(*this == rhs);
	}

	BYTE RGBA::GetR() const
	{
		return m_rgba[0];
	}

	void RGBA::SetR(BYTE r)
	{
		m_rgba[0] = r;
	}

	BYTE RGBA::GetG() const
	{
		return m_rgba[1];
	}

	void RGBA::SetG(BYTE g)
	{
		m_rgba[1] = g;
	}

	BYTE RGBA::GetB() const
	{
		return m_rgba[2];
	}

	void RGBA::SetB(BYTE b)
	{
		m_rgba[2] = b;
	}

	BYTE RGBA::GetA() const
	{
		return m_rgba[3];
	}

	void RGBA::SetA(BYTE a)
	{
		m_rgba[3] = a;
	}

	unsigned int RGBA::GetUnsignedInt() const
	{
		return m_colour;
	}

	Maths::Vec4f RGBA::GetAsNormalisedVec4() const
	{
		return Maths::Vec4f((float)m_rgba[0] / 255.0f, (float)m_rgba[1] / 255.0f, (float)m_rgba[2] / 255.0f, (float)m_rgba[3] / 255.0f);
	}

	RGBA RGBA::GetRandom()
	{
		return RGBA(Maths::Random::InRange<BYTE>(0, 255), Maths::Random::InRange<BYTE>(0, 255), Maths::Random::InRange<BYTE>(0, 255));
	}

	RGBA RGBA::MakeFromHSLA(const HSLA& hsla)
	{
		float h = hsla.GetNormH(), s = hsla.GetNormS(), l = hsla.GetNormL();
		float r, g, b;

		// Achromatic
		if (s == 0.0f)
		{
			r = l;
			g = l;
			b = l;
		}
		else
		{
			float m2;
			if (l <= 0.5f)
			{
				m2 = l * (1.0f + s);
			}
			else
			{
				m2 = l + s - (l * s);
			}
			float m1 = 2.0f * l - m2;

			auto HueToRGB = [](float hue, float m1, float m2)
			{
				if (hue < 0.0f)
				{
					hue += 1.0f;
				}
				else if (hue > 1.0f)
				{
					hue -= 1.0f;
				}

				// Voodoo magic i found on stack overflow
				if (hue < 1.0f / 6.0f)
				{
					return m1 + (m2 - m1) * hue * 6.0f;
				}
				if (hue < 0.5f)
				{
					return m2;
				}
				if (hue < 2.0f / 3.0f)
				{
					return m1 + (m2 - m1) * ((2.0f / 3.0f) - hue) * 6.0f;
				}		
				return m1;
			};

			r = HueToRGB(h + 1.0f / 3.0f, m1, m2);
			g = HueToRGB(h, m1, m2);
			b = HueToRGB(h - 1.0f / 3.0f, m1, m2);
		}

		return RGBA((BYTE)(r * 255.0f), (BYTE)(g * 255.0f), (BYTE)(b * 255.0f), hsla.GetA());
	}

	HSLA::HSLA() :
		m_hue(0),
		m_saturation(0),
		m_lightness(0),
		m_alpha(255)
	{
	}

	HSLA::HSLA(BYTE hue, BYTE saturation, BYTE lightness) :
		m_hue(hue),
		m_saturation(saturation),
		m_lightness(lightness),
		m_alpha(255)
	{
	}

	HSLA::HSLA(BYTE hue, BYTE saturation, BYTE lightness, BYTE alpha) :
		m_hue(hue),
		m_saturation(saturation),
		m_lightness(lightness),
		m_alpha(alpha)
	{
	}

	HSLA::HSLA(const HSLA& src) :
		m_hue(src.m_hue),
		m_saturation(src.m_saturation),
		m_lightness(src.m_lightness),
		m_alpha(src.m_alpha)
	{
	}

	BYTE HSLA::GetH() const
	{
		return m_hue;
	}

	float HSLA::GetNormH() const
	{
		return (float)m_hue / 255.0f;
	}

	void HSLA::SetH(BYTE hue)
	{
		m_hue = hue;
	}

	BYTE HSLA::GetS() const
	{
		return m_saturation;
	}

	float HSLA::GetNormS() const
	{
		return (float)m_saturation / 255.0f;
	}

	void HSLA::SetS(BYTE saturation)
	{
		m_saturation = saturation;
	}

	BYTE HSLA::GetL() const
	{
		return m_lightness;
	}

	float HSLA::GetNormL() const
	{
		return (float)m_lightness / 255.0f;
	}

	void HSLA::SetL(BYTE lightness)
	{
		m_lightness = lightness;
	}

	BYTE HSLA::GetA() const
	{
		return m_alpha;
	}

	void HSLA::SetA(BYTE a)
	{
		m_alpha = a;
	}

	HSLA HSLA::GetRandom()
	{
		return HSLA(Maths::Random::InRange<BYTE>(0, 255), Maths::Random::InRange<BYTE>(0, 255), Maths::Random::InRange<BYTE>(0, 255));
	}

	HSLA HSLA::GetRandomHueAndLightness(BYTE saturation)
	{
		return HSLA(Maths::Random::InRange<BYTE>(0, 255), saturation, Maths::Random::InRange<BYTE>(0, 255));
	}

	HSLA HSLA::GetRandomHueAndSaturation(BYTE lightness)
	{
		return HSLA(Maths::Random::InRange<BYTE>(0, 255), Maths::Random::InRange<BYTE>(0, 255), lightness);
	}

	HSLA HSLA::GetRandomHue(BYTE saturation, BYTE lightness)
	{
		return HSLA(Maths::Random::InRange<BYTE>(0, 255), saturation, lightness);
	}

	HSLA HSLA::MakeFromRGBA(const RGBA& rgba)
	{
		float r = (float)(rgba.GetR()) / 255.0f;
		float g = (float)(rgba.GetG()) / 255.0f;
		float b = (float)(rgba.GetB()) / 255.0f;

		float h, s, l;

		float maxCol = fmaxf(fmaxf(r, g), b);
		float minCol = fminf(fmaxf(r, g), b);

		float colSum = (maxCol + minCol);
		l = colSum / 2.0f;

		if (minCol == maxCol)
		{
			return HSLA(0, (BYTE)(l * 255.0f), 0, rgba.GetA());
		}

		float colRange = (maxCol - minCol);

		if (l <= 0.5f)
		{
			s = colRange / colSum;
		}
		else
		{
			s = colRange / (2.0f - colSum);
		}
			
		float rc = (maxCol - r) / colRange;
		float gc = (maxCol - g) / colRange;
		float bc = (maxCol - b) / colRange;

		if (r == maxCol)
		{
			h = bc - gc;
		}
		else if (g == maxCol)
		{
			h = 2.0f + rc - bc;
		}
		else
		{
			h = 4.0f + gc - rc;
		}

		h = fmodf((h / 6.0f), 1.0f);

		return HSLA((BYTE)(h * 255.0f), (BYTE)(s * 255.0f), (BYTE)(l * 255.0f), rgba.GetA());
	}
}
