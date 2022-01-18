#pragma once

#include "Rect.h"

#include "../Graphics/Data/VertexTypes.h"

#include "../Engine.h"

namespace Firelight::Maths
{
	template<typename T>
	inline Rect<T>::Rect()
	{
	}

	template<typename T>
	inline Rect<T>::Rect(const T& _x, const T& _y, const T& _w, const T& _h) :
		x(_x),
		y(_y),
		w(_w),
		h(_h)
	{
	}

	template<typename T>
	inline void Rect<T>::AddNDCVerticiesToVector(const Rect<T>& destRect, const Rect<T>& sourceRect, std::vector<Graphics::UnlitVertex>& verticies)
	{
	}
}
