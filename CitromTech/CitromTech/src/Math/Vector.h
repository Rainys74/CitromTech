#pragma once

#include "MathCommon.h"

#include "CTL/StaticArray.h"

namespace Citrom::Math
{
    class Vector2
    {
    public:
        float32 Magnitude() const;
        float32 SquaredMagnitude() const;
        Vector2 Normalized() const;

        // Math equations
        Vector2 operator+(const Vector2& vec2B) const { return Vector2{ this->x + vec2B.x, this->y + vec2B.y }; } // Add
        Vector2 operator-(const Vector2& vec2B) const { return Vector2{ this->x - vec2B.x, this->y - vec2B.y }; } // Subtract

        Vector2 operator*(const Vector2& vec2B) const { return Vector2{ this->x * vec2B.x, this->y * vec2B.y }; } // Multiply
        Vector2 operator/(const Vector2& vec2B) const { return Vector2{ this->x / vec2B.x, this->y / vec2B.y }; } // Divide
    public:
        union
        {
            struct
            {
                float32 x;
                float32 y;
            };
            struct
            {
                float32 r;
                float32 g;
            };
        };
    };

    // TODO: maybe template this
    class Vector3
    {
    public:
        float32 Magnitude() const;
        float32 SquaredMagnitude() const;
        Vector3 Normalized() const;

        // Products
        float32 Dot(const Vector3& vec3B) const;
        Vector3 Cross(const Vector3& vec3B) const;

        // Math equations
        Vector3 operator+(const Vector3& vec3B) const { return Vector3{ this->x + vec3B.x, this->y + vec3B.y, this->z + vec3B.z }; } // Add
        Vector3 operator-(const Vector3& vec3B) const { return Vector3{ this->x - vec3B.x, this->y - vec3B.y, this->z - vec3B.z }; } // Subtract

        Vector3 operator*(const Vector3& vec3B) const { return Vector3{ this->x * vec3B.x, this->y * vec3B.y, this->z * vec3B.z }; } // Multiply
        Vector3 operator/(const Vector3& vec3B) const { return Vector3{ this->x / vec3B.x, this->y / vec3B.y, this->z / vec3B.z }; } // Divide
    public:
        union
        {
            struct
            {
                float32 x;
                float32 y;
                float32 z;
            };
            struct
            {
                float32 r;
                float32 g;
                float32 b;
            };
        };
	};

    /*// C = Count, VT = Variable Type
    template<typename C, typename VT>
    class VectorBase
    {
    public:
        float32 Magnitude() const;
        float32 SquaredMagnitude() const;
        // can't really pull this off if inheritance will be used, unless hardcoding every specific function here and
        // doing static asserts is a possibility
        VectorBase<C, VT> Normalized() const;

        // Math equations
        VectorBase<C, VT> operator+(const VectorBase<C, VT>& vecB) const; // Add
        VectorBase<C, VT> operator-(const VectorBase<C, VT>& vecB) const; // Subtract

        VectorBase<C, VT> operator*(const VectorBase<C, VT>& vecB) const; // Multiply
        VectorBase<C, VT> operator/(const VectorBase<C, VT>& vecB) const; // Divide
    public:
        CTL::Array<VT, C> data;
    };*/
}