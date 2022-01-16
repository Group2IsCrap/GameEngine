#pragma once

namespace Firelight::Maths
{
    template<typename T>
    class Rect
    {
    public:
        Rect();
        Rect(const T& _x, const T& _y, const T& _w, const T& _h);

    public:
        T x, y, w, h;
    };

    typedef Rect<float> Rectf;
}

#include "Rect.inl"
