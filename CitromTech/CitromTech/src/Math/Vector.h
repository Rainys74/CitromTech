#pragma once

#include "MathCommon.h"

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
        float32 Magnitude();
        float32 SquaredMagnitude();
        Vector3 Normalized();

        // Math equations
        Vector3 Add(const Vector3& vec3B) {}
        Vector3 Subtract(const Vector3& vec3B){}

        Vector3 Multiply(const Vector3& vec3B){}
        Vector3 Divide(const Vector3& vec3B){}
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
}