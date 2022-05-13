#pragma once

#include "rapidjson/prettywriter.h"

namespace Firelight::Maths
{
    template<typename T>
    class Vec2
    {
    public:
        Vec2();
        Vec2(const T& val);
        Vec2(const T& _x, const T& _y);
        ~Vec2();

        // Operator overloads
        Vec2<T>        operator+(const Vec2<T>& vector) const;
        void           operator+=(const Vec2<T>& vector);
        Vec2<T>        operator+(const T scalar) const;
                       
        Vec2<T>        operator-(const Vec2<T>& vector) const;
        void           operator-=(const Vec2<T>& vector);
        Vec2<T>        operator-(const T scalar) const;
                       
        Vec2<T>        operator*(const Vec2<T>& vector) const;
        Vec2<T>        operator*(const T scalar) const;
        void           operator*=(const T scalar);
                       
        Vec2<T>        operator/(const Vec2<T>& vector) const;
        Vec2<T>        operator/(const T scalar) const;
        void           operator/=(const T scalar);

        bool           operator==(const Vec2<T>& vector) const;
        bool           operator==(const T scalar) const;

        T&             operator[](int index);

        // Member functions
        T              Length() const;
        T              LengthSquared() const;
        void           Normalise();
        T              Dot(const Vec2<T>& vector) const;

        // Static functions
        static Vec2<T> Normalise(const Vec2<T>& vector);
        static T       Dot(const Vec2<T>& vector1, const Vec2<T>& vector2);

        static T       Length(const Vec2<T>& vector);
        static T       LengthSquared(const Vec2<T>& vector);

        static T       Dist(const Vec2<T>& vector1, const Vec2<T>& vector2);
        static T       DistSquared(const Vec2<T>& vector1, const Vec2<T>& vector2);

        static Vec2<T> Reflect(const Vec2<T>& incident, const Vec2<T>& normal);
        static Vec2<T> Right(const Vec2<T>& vector);

        static Vec2<T> GetRandomVector();
        static Vec2<T> GetRandomDirection();
        static Vec2<T> Lerp(const Vec2<T>& vector1, const Vec2<T>& Vec2, T delta);

        static Vec2<T> RotateAroundPoint(const Vec2<T>& toRotate, const Vec2<T>& point, double rotation, bool correctNDC = false, float aspect = 0.0f);

        void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

    public:
        T x, y;
    };

    typedef Vec2<float> Vec2f;
    typedef Vec2<int> Vec2i;
}

#include "Vec2.inl"
