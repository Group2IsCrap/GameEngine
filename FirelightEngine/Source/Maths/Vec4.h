#pragma once

#include <DirectXMath.h>

// Purely just a storage class at the moment

namespace Firelight::Maths
{
    template<typename T>
    class Vec4
    {
    public:
        Vec4();
        Vec4(const T& val);
        Vec4(const T& _x, const T& _y, const T& _z, const T& _w);
        ~Vec4();

        // XMVECTOR
        DirectX::XMVECTOR GetAsXMVECTOR() const;
        DirectX::XMFLOAT4 GetAsXMFLOAT4() const;

        static Vec4<T>    MakeFromXMVECTOR(const DirectX::XMVECTOR& vector);
        static Vec4<T>    MakeFromXMFLOAT4(const DirectX::XMFLOAT4& float4);

    public:
        T x, y, z, w;
    };

    typedef Vec4<float> Vec4f;
    typedef Vec4<int> Vec4i;
}

#include "Vec4.inl"
