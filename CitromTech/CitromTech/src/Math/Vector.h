#pragma once

#include "MathCommon.h"

namespace Citrom::Math
{
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