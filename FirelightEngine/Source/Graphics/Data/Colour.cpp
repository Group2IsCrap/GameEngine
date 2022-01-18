#pragma once

#include "Colour.h"

namespace Firelight::Graphics
{
	Colour::Colour() :
		m_colour(0)
	{
	}

	Colour::Colour(BYTE assignAllVal) :
		Colour(assignAllVal, assignAllVal, assignAllVal)
	{
	}

	Colour::Colour(unsigned int numVal) :
		m_colour(numVal)
	{
	}

	Colour::Colour(BYTE r, BYTE g, BYTE b) :
		Colour(r, g, b, 255)
	{
	}

	Colour::Colour(BYTE r, BYTE g, BYTE b, BYTE a) :
		m_rgba{r, g, b, a}
	{
	}

	Colour::Colour(const Colour& src) :
		m_colour(src.m_colour)
	{
	}

	Colour& Colour::operator=(const Colour& src)
	{
		m_colour = src.m_colour;
		return *this;
	}

	bool Colour::operator==(const Colour& rhs) const
	{
		return (m_colour == rhs.m_colour);
	}

	bool Colour::operator!=(const Colour& rhs) const
	{
		return !(*this == rhs);
	}

	constexpr BYTE Colour::GetR() const
	{
		return m_rgba[0];
	}

	void Colour::SetR(BYTE r)
	{
		m_rgba[0] = r;
	}

	constexpr BYTE Colour::GetG() const
	{
		return m_rgba[1];
	}

	void Colour::SetG(BYTE g)
	{
		m_rgba[1] = g;
	}

	constexpr BYTE Colour::GetB() const
	{
		return m_rgba[2];
	}

	void Colour::SetB(BYTE b)
	{
		m_rgba[2] = b;
	}

	constexpr BYTE Colour::GetA() const
	{
		return m_rgba[3];
	}

	void Colour::SetA(BYTE a)
	{
		m_rgba[3] = a;
	}

	unsigned int Colour::GetUnsignedInt() const
	{
		return m_colour;
	}
}