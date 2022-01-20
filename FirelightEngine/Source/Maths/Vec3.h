#pragma once

#include <DirectXMath.h>

namespace Firelight::Maths
{
    template<typename T>
    class Vec3
    {
    public:
        Vec3();
        Vec3(const T& val);
        Vec3(const T& _x, const T& _y, const T& _z);
        ~Vec3();

        // Operator overloads
        Vec3<T>           operator+(const Vec3<T>& vector) const;
        void              operator+=(const Vec3<T>& vector);
        Vec3<T>           operator+(const T scalar) const;

        Vec3<T>           operator-(const Vec3<T>& vector) const;
        void              operator-=(const Vec3<T>& vector);
        Vec3<T>           operator-(const T scalar) const;

        Vec3<T>           operator*(const Vec3<T>& vector) const;
        Vec3<T>           operator*(const T scalar) const;
        void              operator*=(const T scalar);

        Vec3<T>           operator/(const T scalar) const;
        void              operator/=(const T scalar);

        bool              operator==(const Vec3<T>& vector) const;
        bool              operator==(const T scalar) const;

        // Member functions
        T                 Length() const;
        T                 LengthSquared() const;
        void              Normalise();
        T                 Dot(const Vec3<T>& vector) const;
        Vec3<T>           Cross(const Vec3<T>& vector);

        // XMVECTOR
        DirectX::XMVECTOR GetAsXMVECTOR() const;
        DirectX::XMFLOAT3 GetAsXMFLOAT3() const;

        // Static functions
        static Vec3<T>    Normalise(const Vec3<T>& vector);
        static T          Dot(const Vec3<T>& vector1, const Vec3<T>& vector2);
        static Vec3<T>    Cross(const Vec3<T>& vector1, const Vec3<T>& vector2);
                          
        static T          Length(const Vec3<T>& vector);
        static T          LengthSquared(const Vec3<T>& vector);
                          
        static T          Dist(const Vec3<T>& vector1, const Vec3<T>& vector2);
        static T          DistSquared(const Vec3<T>& vector1, const Vec3<T>& vector2);
                          
        static Vec3<T>    Reflect(const Vec3<T>& incident, const Vec3<T>& normal);
                          
        static Vec3<T>    GetRandomVector();
        static Vec3<T>    GetRandomDirection();
        static Vec3<T>    Lerp(const Vec3<T>& vector1, const Vec3<T>& Vec3, T delta);

        // XMVECTOR
        static Vec3<T>    MakeFromXMVECTOR(const DirectX::XMVECTOR& vector);
        static Vec3<T>    MakeFromXMFLOAT3(const DirectX::XMFLOAT3& float3);

    public:
        T x, y, z;
    };

    typedef Vec3<float> Vec3f;
    typedef Vec3<int> Vec3i;
}

#include "Vec3.inl"
