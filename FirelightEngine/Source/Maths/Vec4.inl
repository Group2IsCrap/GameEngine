#pragma once

#include "Vec4.h"

namespace Firelight::Maths
{
    template<typename T>
    inline Vec4<T>::Vec4() :
        x((T)0),
        y((T)0),
        z((T)0),
        w((T)0)
    {
    }

    template<typename T>
    inline Vec4<T>::Vec4(const T& val) :
        x(val),
        y(val),
        z(val),
        w(val)
    {
    }

    template<typename T>
    inline Vec4<T>::Vec4(const T& _x, const T& _y, const T& _z, const T& _w) :
        x(_x),
        y(_y),
        z(_z),
        w(_w)
    {
    }

    template<typename T>
    inline Vec4<T>::~Vec4()
    {
    }

    template<typename T>
    inline DirectX::XMVECTOR Vec4<T>::GetAsXMVECTOR() const
    {
        return DirectX::XMVectorSet(x, y, z, w);
    }

    template<typename T>
    inline DirectX::XMFLOAT4 Vec4<T>::GetAsXMFLOAT4() const
    {
        return DirectX::XMFLOAT4(x, y, z, w);
    }

    template<typename T>
    inline Vec4<T> Vec4<T>::MakeFromXMVECTOR(const DirectX::XMVECTOR& vector)
    {
        return Vec4<T>((T)DirectX::XMVectorGetX(vector), (T)DirectX::XMVectorGetY(vector), (T)DirectX::XMVectorGetZ(vector), (T)DirectX::XMVectorGetW(vector));
    }

    template<typename T>
    inline Vec4<T> Vec4<T>::MakeFromXMFLOAT4(const DirectX::XMFLOAT4& float4)
    {
        return Vec4<T>(float4.x, float4.y, float4.z, float4.w);
    }
}