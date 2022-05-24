#pragma once

#include "Vec3.h"

#include <DirectXMath.h>
#include <../Source/Engine.h>

#include "Random.h"
#include "../Utils/ErrorManager.h"

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
        x(val),
        y(val),
        z(val)
    {
    }

    template<typename T>
    inline Vec3<T>::Vec3(const T& _x, const T& _y, const T& _z) :
        x(_x),
        y(_y),
        z(_z)
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
    inline T& Vec3<T>::operator[](int index)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }

        ERROR_STANDARD("Index out of range for vector 3");
        return x;
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
    inline Vec3<T> Vec3<T>::Invert(const Vec3<T>& vector1)
    {
        return Vec3<T>(1 / vector1.x, 1 / vector1.y, 1 / vector1.z);
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
    inline Vec3<T> Vec3<T>::SmoothDamp(const Vec3<T>& Vector1, const Vec3<T>& Vector2, const Vec3<T>& currentVelocity, float smoothTime, float maxSpeed)
    {
        float output_x = 0.0f;
        float output_y = 0.0f;
        float output_z = 0.0f;

        smoothTime = (0.0001f > smoothTime) ? 0.0001f : smoothTime;
        float omega = 2.0f / smoothTime;

        float x = omega * Firelight::Engine::Instance().GetTime().GetDeltaTime();
        float exp = 1.0f / ((1.0f + x + (0.48f * std::powf(x, 2)) + (0.235f * std::powf(x, 3))));

        float change_x = Vector1.x - Vector2.x;
        float change_y = Vector1.y - Vector2.y;
        float change_z = Vector1.z - Vector2.z;
        Vec3f originalTo = Vector2;

        float maxChange = maxSpeed * smoothTime;

        float maxChangeSq = maxChange * maxChange;
        float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;

        if (sqrmag > maxChangeSq)
        {
            float mag = (float)std::sqrtf(sqrmag);
            change_x = change_x / mag * maxChange;
            change_y = change_y / mag * maxChange;
            change_z = change_z / mag * maxChange;

        }

        Vector2.x = Vector1.x - change_x;
        Vector2.y = Vector1.y - change_y;
        Vector2.z = Vector1.z - change_z;

        float temp_x = (currentVelocity.x + omega * change_x) * Firelight::Engine::Instance().GetTime().GetDeltaTime();
        float temp_y = (currentVelocity.y + omega * change_y) * Firelight::Engine::Instance().GetTime().GetDeltaTime();
        float temp_z = (currentVelocity.z + omega * change_z) * Firelight::Engine::Instance().GetTime().GetDeltaTime();

        currentVelocity.x = (currentVelocity.x - omega * temp_x) * exp;
        currentVelocity.y = (currentVelocity.y - omega * temp_y) * exp;
        currentVelocity.z = (currentVelocity.z - omega * temp_z) * exp;

        output_x = Vector2.x + (change_x + temp_x) * exp;
        output_y = Vector2.y + (change_y + temp_y) * exp;
        output_z = Vector2.z + (change_z + temp_z) * exp;

        float origMinusCurrent_x = originalTo.x - Vector1.x;
        float origMinusCurrent_y = originalTo.y - Vector1.y;
        float origMinusCurrent_z = originalTo.z - Vector1.z;
        float outMinusOrig_x = output_x - originalTo.x;
        float outMinusOrig_y = output_y - originalTo.y;
        float outMinusOrig_z = output_z - originalTo.z;

        if ((origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y + origMinusCurrent_z * outMinusOrig_z) > 0)
        {
            output_x = originalTo.x;
            output_y = originalTo.y;
            output_z = originalTo.z;

            currentVelocity.x = (output_x - originalTo.x) / Firelight::Engine::Instance().GetTime().GetDeltaTime();
            currentVelocity.y = (output_y - originalTo.y) / Firelight::Engine::Instance().GetTime().GetDeltaTime();
            currentVelocity.z = (output_z - originalTo.z) / Firelight::Engine::Instance().GetTime().GetDeltaTime();
        }

        return Vec3f(output_x, output_y, output_z);
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
