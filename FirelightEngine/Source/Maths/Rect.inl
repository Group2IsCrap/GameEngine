#pragma once

#include "Rect.h"

#include "../Graphics/Data/VertexTypes.h"

namespace Firelight::Maths
{
	template<typename T>
	inline Rect<T>::Rect() :
		x((T)0),
		y((T)0),
		w((T)0),
		h((T)0)
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
	inline Rect<T> Rect<T>::CreateNDCRectFromPixelRect(const Maths::Vec2f& dimensions) const
	{
		Rect<T> returnRect;

		returnRect.x = (x / dimensions.x) * 2.0f - 1.0f;
		returnRect.y = (1.0f - ((y + h) / dimensions.y)) * 2.0f - 1.0f;
		returnRect.w = (w / dimensions.x) * 2.0f;
		returnRect.h = (h / dimensions.y) * 2.0f;

		return returnRect;
	}

	template<typename T>
	inline bool Rect<T>::IsValid() const
	{
		return w >= (T)0 && h >= (T)0;
	}

	template<typename T>
	inline Vec2<T> Rect<T>::GetCentre() const
	{
		return Vec2<T>(x + w * 0.5, y + h * 0.5);
	}

	template<typename T>
	inline Vec2<T> Rect<T>::GetCentreNDC() const
	{
		return Vec2<T>(x + w / 2, y + h / 2);
	}

	template<typename T>
	inline Rect<T> Rect<T>::CreateNDCRectInWorldRect(const Rect<T>& smallRect, const Rect<T>& worldRect)
	{
		return Rect<T>(
			((smallRect.x - worldRect.x) / worldRect.w) * 2.0f - 1.0f,
			((smallRect.y - worldRect.y) / worldRect.h) * 2.0f - 1.0f,
			(smallRect.w / worldRect.w) * 2.0f,
			(smallRect.h / worldRect.h) * 2.0f
			);
	}

	template<typename T>
	inline bool Rect<T>::IsOverlappingRect(const Rect<T>& otherRect) const
	{
		if (x > otherRect.x + otherRect.w || x + w < otherRect.x || y > otherRect.y + otherRect.h || y + h < otherRect.y)
		{
			return false;
		}
		return true;
	}
}
