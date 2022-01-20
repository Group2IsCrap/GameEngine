#pragma once

#include "Vec3.h"

#include <DirectXMath.h>

#include "Random.h"

namespace Firelight::Maths
{
    template<typename T>
    inline Vec3<T>::Vec3() :
        x((T)0),
        y((T)0),
        z((T)0)
    {
    }

    template<typename T>
    inline Vec3<T>::Vec3(const T& val) :
        x((T)val),
        y((T)val),
        z((T)val)
    {
    }

    template<typename T>
    inline Vec3<T>::Vec3(const T& _x, const T& _y, const T& _z) :
        x((T)_x),
        y((T)_y),
        z((T)_z)
    {
    }

    template<typename T>
    inline Vec3<T>::~Vec3()
    {
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator+(const Vec3<T>& vector) const
    {
        return Vec3<T>(x + vector.x, y + vector.y, z + vector.z);
    }

    template<typename T>
    inline void Vec3<T>::operator+=(const Vec3<T>& vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator+(const T scalar) const
    {
        return Vec3<T>(x + scalar, y + scalar, z + scalar);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator-(const Vec3<T>& vector) const
    {
        return Vec3<T>(x - vector.x, y - vector.y, z - vector.z);
    }

    template<typename T>
    inline void Vec3<T>::operator-=(const Vec3<T>& vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator-(const T scalar) const
    {
        return Vec3<T>(x - scalar, y - scalar, z - scalar);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator*(const Vec3<T>& vector) const
    {
        return Vec3<T>(x * vector.x, y * vector.y, z * vector.z);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator*(const T scalar) const
    {
        return Vec3<T>(x * scalar, y * scalar, z * scalar);
    }

    template<typename T>
    inline void Vec3<T>::operator*=(const T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::operator/(const T scalar) const
    {
        return Vec3<T>(x / scalar, y / scalar, z / scalar);
    }

    template<typename T>
    inline void Vec3<T>::operator/=(const T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }

    template<typename T>
    inline bool Vec3<T>::operator==(const Vec3<T>& vector) const
    {
        return x == vector.x && y == vector.y && z == vector.z;
    }

    template<typename T>
    inline bool Vec3<T>::operator==(const T scalar) const
    {
        return x == scalar && y == scalar && z == scalar;
    }

    template<typename T>
    inline T Vec3<T>::Length() const
    {
        return sqrt(LengthSquared());
    }

    template<typename T>
    inline T Vec3<T>::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    template<typename T>
    inline void Vec3<T>::Normalise()
    {
        *this = Vec3<T>::Normalise(*this);
    }

    template<typename T>
    inline T Vec3<T>::Dot(const Vec3<T>& vector) const
    {
        return Vec3<T>::Dot(*this, vector);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::Cross(const Vec3<T>& vector)
    {
        return Vec3<T>::Cross(*this, vector);
    }

    template<typename T>
    inline DirectX::XMVECTOR Vec3<T>::GetAsXMVECTOR() const
    {
        return DirectX::XMVectorSet((float)x, (float)y, (float)z, 0.0f);
    }

    template<typename T>
    inline  DirectX::XMFLOAT3 Vec3<T>::GetAsXMFLOAT3() const
    {
        return DirectX::XMFLOAT3((float)x, (float)y, (float)z);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::Normalise(const Vec3<T>& vector)
    {
        T lengthSquared = vector.LengthSquared();

        if (lengthSquared < (T)0.001)
        {
            return Vec3<T>(0, -1, 0);
        }

        // Only sqrt for non-zero input
        T length = sqrt(lengthSquared);

        Vec3<T> normalisedVector;
        normalisedVector.x = vector.x / length;
        normalisedVector.y = vector.y / length;
        normalisedVector.z = vector.z / length;

        return normalisedVector;
    }

    template<typename T>
    inline T Vec3<T>::Dot(const Vec3<T>& vector1, const Vec3<T>& vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::Cross(const Vec3<T>& vector1, const Vec3<T>& vector2)
    {
        return Vec3<T>(vector1.y * vector2.z - vector1.z * vector2.y,
            vector1.z * vector2.x - vector1.x * vector2.z,
            vector1.x * vector2.y - vector1.y * vector2.x);
    }

    template<typename T>
    inline T Vec3<T>::Length(const Vec3<T>& vector)
    {
        return vector.Length();
    }

    template<typename T>
    inline T Vec3<T>::LengthSquared(const Vec3<T>& vector)
    {
        return vector.LengthSquared();
    }

    template<typename T>
    inline T Vec3<T>::Dist(const Vec3<T>& vector1, const Vec3<T>& vector2)
    {
        return sqrt(Vec3<T>::DistSquared(vector1, vector2));
    }

    template<typename T>
    inline T Vec3<T>::DistSquared(const Vec3<T>& vector1, const Vec3<T>& vector2)
    {
        Vec3<T> diffVector = vector1 - vector2;
        return diffVector.LengthSquared();
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::Reflect(const Vec3<T>& incident, const Vec3<T>& normal)
    {
        return incident - normal * 2.0f * Vec3<T>::Dot(normal, incident);
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::GetRandomVector()
    {
        return Vec3<T>(Random::NegOneToOne<T>(), Random::NegOneToOne<T>(), Random::NegOneToOne<T>());
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::GetRandomDirection()
    {
        return Vec3<T>::Normalise(Vec3<T>::GetRandomVector());
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::Lerp(const Vec3<T>& vector1, const Vec3<T>& vector2, T delta)
    {
        return vector1 + (vector2 - vector1) * delta;
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::MakeFromXMVECTOR(const DirectX::XMVECTOR& vector)
    {
        return Vec3<T>((T)DirectX::XMVectorGetX(vector), (T)DirectX::XMVectorGetY(vector), (T)DirectX::XMVectorGetZ(vector));
    }

    template<typename T>
    inline Vec3<T> Vec3<T>::MakeFromXMFLOAT3(const DirectX::XMFLOAT3& float3)
    {
        return Vec3<T>(float3.x, float3.y, float3.z);
    }
}
