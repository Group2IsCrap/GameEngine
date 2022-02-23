#pragma once

#include "../Graphics/Data/VertexTypes.h"

#include "Vec2.h"

namespace Firelight::Maths
{
    template<typename T>
    class Rect
    {
    public:
        Rect();
        Rect(const T& _x, const T& _y, const T& _w, const T& _h);

        Rect<T> CreateNDCRectFromPixelRect(const Maths::Vec2f& dimensions) const;
        bool    IsValid() const;

        Vec2<T> GetCentre() const;
        Vec2<T> GetCentreNDC() const;

        static Rect<T> CreateNDCRectInWorldRect(const Rect<T>& smallRect, const Rect<T>& worldRect);

    public:
        T x, y, w, h;
    };

    typedef Rect<float> Rectf;
    typedef Rect<int> Recti;
}

#include "Rect.inl"
