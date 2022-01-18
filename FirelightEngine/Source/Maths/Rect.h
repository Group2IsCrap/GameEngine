#pragma once

#include "../Graphics/Data/VertexTypes.h"

namespace Firelight::Maths
{
    template<typename T>
    class Rect
    {
    public:
        Rect();
        Rect(const T& _x, const T& _y, const T& _w, const T& _h);

        Rect<T> CreateNDCRectFromPixelRect() const;

    public:
        T x, y, w, h;
    };

    typedef Rect<float> Rectf;
    typedef Rect<int> Recti;
}

#include "Rect.inl"
