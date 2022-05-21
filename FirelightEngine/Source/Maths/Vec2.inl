#pragma once

#include "Vec2.h"

#include <math.h>

#include "Random.h"
#include "../Utils/ErrorManager.h"

namespace Firelight::Maths
{
    template<typename T>
    inline Vec2<T>::Vec2() :
        x((T)0),
        y((T)0)
    {
    }

    template<typename T>
    inline Vec2<T>::Vec2(const T& val) :
        x(val),
        y(val)
    {
    }

    template<typename T>
    inline Vec2<T>::Vec2(const T& _x, const T& _y) :
        x(_x),
        y(_y)
    {
    }

    template<typename T>
    inline Vec2<T>::~Vec2()
    {
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator+(const Vec2<T>& vector) const
    {
        return Vec2<T>(x + vector.x, y + vector.y);
    }

    template<typename T>
    inline void Vec2<T>::operator+=(const Vec2<T>& vector)
    {
        x += vector.x;
        y += vector.y;
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator+(const T scalar) const
    {
        return Vec2<T>(x + scalar, y + scalar);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator-(const Vec2<T>& vector) const
    {
        return Vec2<T>(x - vector.x, y - vector.y);
    }

    template<typename T>
    inline void Vec2<T>::operator-=(const Vec2<T>& vector)
    {
        x -= vector.x;
        y -= vector.y;
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator-(const T scalar) const
    {
        return Vec2<T>(x - scalar, y - scalar);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator*(const Vec2<T>& vector) const
    {
        return Vec2<T>(x * vector.x, y * vector.y);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator*(const T scalar) const
    {
        return Vec2<T>(x * scalar, y * scalar);
    }

    template<typename T>
    inline void Vec2<T>::operator*=(const T scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator/(const Vec2<T>& vector) const
    {
        return Vec2<T>(x / vector.x, y / vector.y);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::operator/(const T scalar) const
    {
        return Vec2<T>(x / scalar, y / scalar);
    }

    template<typename T>
    inline void Vec2<T>::operator/=(const T scalar)
    {
        x /= scalar;
        y /= scalar;
    }

    template<typename T>
    inline bool Vec2<T>::operator==(const Vec2<T>& vector) const
    {
        return x == vector.x && y == vector.y;
    }

    template<typename T>
    inline bool Vec2<T>::operator==(const T scalar) const
    {
        return x == scalar && y == scalar;
    }

    template<typename T>
    inline T& Vec2<T>::operator[](int index)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        }

        ERROR_STANDARD("Index out of range for vector 2");
        return x;
    }

    template<typename T>
    inline T Vec2<T>::Length() const
    {
        return sqrt(LengthSquared());
    }

    template<typename T>
    inline T Vec2<T>::LengthSquared() const
    {
        return x * x + y * y;
    }

    template<typename T>
    inline void Vec2<T>::Normalise()
    {
        *this = Vec2<T>::Normalise(*this);
    }

    template<typename T>
    inline T Vec2<T>::Dot(const Vec2<T>& vector) const
    {
        return Vec2<T>::Dot(*this, vector);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::Normalise(const Vec2<T>& vector)
    {
        T lengthSquared = vector.LengthSquared();

        if (lengthSquared < (T)0.001)
        {
            return Vec2<T>(0, -1);
        }

        // Only sqrt for non-zero input
        T length = sqrt(lengthSquared);

        Vec2<T> normalisedVector;
        normalisedVector.x = vector.x / length;
        normalisedVector.y = vector.y / length;

        return normalisedVector;
    }

    template<typename T>
    inline T Vec2<T>::Dot(const Vec2<T>& vector1, const Vec2<T>& vector2)
    {
        return (vector1.x * vector2.x) + (vector1.y * vector2.y);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::Invert(const Vec2<T>& vector1)
    {
        return Vec2<T>(1 / vector1.x, 1 / vector1.y);
    }

    template<typename T>
    inline T Vec2<T>::Length(const Vec2<T>& vector)
    {
        return vector.Length();
    }

    template<typename T>
    inline T Vec2<T>::LengthSquared(const Vec2<T>& vector)
    {
        return vector.LengthSquared();
    }

    template<typename T>
    inline T Vec2<T>::Dist(const Vec2<T>& vector1, const Vec2<T>& vector2)
    {
        return sqrt(Vec2<T>::DistSquared(vector1, vector2));
    }

    template<typename T>
    inline T Vec2<T>::DistSquared(const Vec2<T>& vector1, const Vec2<T>& vector2)
    {
        Vec2<T> diffVector = vector1 - vector2;
        return diffVector.LengthSquared();
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::Reflect(const Vec2<T>& incident, const Vec2<T>& normal)
    {
        return incident - normal * 2.0f * Vec2<T>::Dot(normal, incident);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::Right(const Vec2<T>& vector)
    {
        return Vec2<T>(-vector.y, vector.x);
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::GetRandomVector()
    {
        return Vec2<T>(Random::NegOneToOne<T>(), Random::NegOneToOne<T>());
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::GetRandomDirection()
    {
        return Vec2<T>::Normalise(Vec2<T>::GetRandomVector());
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::Lerp(const Vec2<T>& vector1, const Vec2<T>& vector2, T delta)
    {
        return vector1 + (vector2 - vector1) * delta;
    }

    template<typename T>
    inline Vec2<T> Vec2<T>::RotateAroundPoint(const Vec2<T>& toRotate, const Vec2<T>& point, double rotation, bool correctNDC, float aspect)
    {
        Vec2<T> offset = toRotate - point;
        Vec2<T> rotatedOffset;

        if (correctNDC)
        {
            offset.y /= aspect;
        }

        rotatedOffset.x = (T)(cos(rotation) * (double)offset.x - sin(rotation) * (double)offset.y);
        rotatedOffset.y = (T)(sin(rotation) * (double)offset.x + cos(rotation) * (double)offset.y);

        if (correctNDC)
        {
            rotatedOffset.y *= aspect;
        }

        return point + rotatedOffset;
    }
}
