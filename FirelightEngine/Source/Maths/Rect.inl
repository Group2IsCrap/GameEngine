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
	inline Rect<T> Rect<T>::CreateNDCRectFromPixelRect() const
	{
		const Maths::Vec2f& dimensions = Engine::Instance().GetWindowDimensionsFloat();
		Rect<T> returnRect(0);

		returnRect.x = (x / dimensions.x) * 2.0f - 1.0f;
		returnRect.y = (1.0f - (y / dimensions.y)) * 2.0f - 1.0f;
		returnRect.w = (w / dimensions.x) * 2.0f;
		returnRect.h = (h / dimensions.y) * 2.0f;

		return returnRect;
	}
}
