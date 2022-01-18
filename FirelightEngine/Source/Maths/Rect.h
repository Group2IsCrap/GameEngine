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

        static void AddNDCVerticiesToVector(const Rect<T>& dest, const Rect<T>& source, std::vector<Graphics::UnlitVertex>& verticies);

    public:
        T x, y, w, h;
    };

    typedef Rect<float> Rectf;
    typedef Rect<int> Recti;
}

#include "Rect.inl"
