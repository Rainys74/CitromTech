#pragma once

#include "MathCommon.h"

#include "CTL/StaticArray.h"
#include "CTL/TypeChecking.h"

namespace Citrom::Math
{
    class Vector2
    {
    public:
        Vector2()
            : x(0), y(0) {}
        Vector2(const Vector2&) = default;
        Vector2(float32 vecX, float32 vecY)
            : x(vecX), y(vecY) {}

        float32 Magnitude() const;
        float32 SquaredMagnitude() const;
        Vector2 Normalized() const;

        // Math equations
        Vector2 operator+(const Vector2& vec2B) const { return Vector2{ this->x + vec2B.x, this->y + vec2B.y }; } // Add
        Vector2 operator-(const Vector2& vec2B) const { return Vector2{ this->x - vec2B.x, this->y - vec2B.y }; } // Subtract

        Vector2 operator*(const Vector2& vec2B) const { return Vector2{ this->x * vec2B.x, this->y * vec2B.y }; } // Multiply
        Vector2 operator/(const Vector2& vec2B) const { return Vector2{ this->x / vec2B.x, this->y / vec2B.y }; } // Divide

        /*
         * Works because stack memory is stored contiguously,
         * No need to account for byte sizes (sizeof() operator) because if the type is known
         * the compiler already calculates it correctly
        */
        FORCE_INLINE float32& operator[](size_t index) { return *(float32*)(&x + index); }
        FORCE_INLINE const float32& operator[](size_t index) const { return *(float32*)(&x + index); }
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
        //Vector3() = default;
        Vector3()
            : x(0), y(0), z(0) {}
        Vector3(const Vector3&) = default;
        Vector3(float32 vecX, float32 vecY, float32 vecZ)
            : x(vecX), y(vecY), z(vecZ) {}

        float32 Magnitude() const;
        float32 SquaredMagnitude() const;
        Vector3 Normalized() const;

        // Presets
        FORCE_INLINE static Vector3 Zero()      { return Vector3();                 }

        FORCE_INLINE static Vector3 Right()     { return Vector3(1.0f, 0.0f, 0.0f); }
        FORCE_INLINE static Vector3 Up()        { return Vector3(0.0f, 1.0f, 0.0f); }
        FORCE_INLINE static Vector3 Forward()   { return Vector3(0.0f, 0.0f, 1.0f); }

        FORCE_INLINE static Vector3 Left() { return -Right(); }
        FORCE_INLINE static Vector3 Down() { return -Up(); }
        FORCE_INLINE static Vector3 Back() { return -Forward(); }

        // Products
        float32 Dot(const Vector3& vec3B) const;
        Vector3 Cross(const Vector3& vec3B) const;

        FORCE_INLINE Vector3 RadToDegrees() const { return Vector3(RadiansToDegrees(this->x), RadiansToDegrees(this->y), RadiansToDegrees(this->z)); }
        FORCE_INLINE Vector3 DegToRadians() const { return Vector3(DegreesToRadians(this->x), DegreesToRadians(this->y), DegreesToRadians(this->z)); }

        // Math equations
        FORCE_INLINE Vector3 operator+(const Vector3& vec3B) const { return Vector3{ this->x + vec3B.x, this->y + vec3B.y, this->z + vec3B.z }; } // Add
        FORCE_INLINE Vector3 operator-(const Vector3& vec3B) const { return Vector3{ this->x - vec3B.x, this->y - vec3B.y, this->z - vec3B.z }; } // Subtract

        FORCE_INLINE Vector3 operator*(const Vector3& vec3B) const { return Vector3{ this->x * vec3B.x, this->y * vec3B.y, this->z * vec3B.z }; } // Multiply
        FORCE_INLINE Vector3 operator/(const Vector3& vec3B) const { return Vector3{ this->x / vec3B.x, this->y / vec3B.y, this->z / vec3B.z }; } // Divide

        // Float32
        FORCE_INLINE Vector3 operator+(const float32 value) const { return Vector3{ this->x + value, this->y + value, this->z + value }; } // Add
        FORCE_INLINE Vector3 operator-(const float32 value) const { return Vector3{ this->x - value, this->y - value, this->z - value }; } // Subtract

        FORCE_INLINE Vector3 operator*(const float32 value) const { return Vector3{ this->x * value, this->y * value, this->z * value }; } // Multiply
        FORCE_INLINE Vector3 operator/(const float32 value) const { return Vector3{ this->x / value, this->y / value, this->z / value }; } // Divide

        FORCE_INLINE Vector3 operator-() const { return Vector3{ -x, -y, -z }; }

        inline Vector3& operator+=(const Vector3& vec3B) { this->x += vec3B.x; this->y += vec3B.y; this->z += vec3B.z; return *this; }
        inline Vector3& operator-=(const Vector3& vec3B) { this->x -= vec3B.x; this->y -= vec3B.y; this->z -= vec3B.z; return *this; }

        FORCE_INLINE bool operator==(const Vector3& vec3B) const { return (this->x == vec3B.x && this->y == vec3B.y && this->z == vec3B.z); }
        FORCE_INLINE bool operator!=(const Vector3& vec3B) const { return !(*this == vec3B); }

        // Should only be used for debugging purposes
        std::string ToString() const;

        // Referencing
        /*
         * Works because stack memory is stored contiguously,
         * No need to account for byte sizes (sizeof() operator) because if the type is known
         * the compiler already calculates it correctly
        */
        FORCE_INLINE float32& operator[](size_t index) { return *(float32*)(&x + index); }
        FORCE_INLINE const float32& operator[](size_t index) const { return *(float32*)(&x + index); }
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

    // C = Count, VT = Variable Type
    template<size_t C, typename VT>
    class VectorBase
    {
    public:
        using VectorT = VectorBase<C, VT>;

        VT Magnitude() const
        {
            VT magnitude = 0;
            for (VT value : data)
                magnitude += value * value;
            static_assert(CTL::IsSame<VT, float32>::value || CTL::IsSame<VT, float64>::value, "SquareRoot() is not supported by the current type of variable!");
            return SquareRoot(magnitude);
        }
        VT SquaredMagnitude() const
        {
            VT magnitude = 0;
            for (VT value : data)
                magnitude += value * value;
            return magnitude;
        }
        VectorT Normalized() const
        {
            VectorT normalized = {};
            VT magnitude = Magnitude();

            if (magnitude == 0)
                return normalized;

            for (size_t i = 0; i < data.Length(); i++)
            {
                normalized.data[i] = data[i] / magnitude;

                if (IS_NAN(normalized.data[i]))
                    normalized.data[i] = 0;
            }

            return normalized;
        }

        VT Dot(const VectorT& vecB) const
        {
            VT productResult = 0;
            for (size_t i = 0; i < data.Length(); ++i)
                productResult += data[i] * vecB.data[i];
            return productResult;
        }

        // Vector3
        VectorBase<3, VT> Cross(const VectorBase<3, VT>& vec3B) const
        {
            static_assert(C == 3, "Cross Product can only be used on Vector3's!");
            return VectorBase<3, VT>{
                    data[1] * vec3B.data[2] - data[2] * vec3B.data[1],
                    data[2] * vec3B.data[0] - data[0] * vec3B.data[2],
                    data[0] * vec3B.data[1] - data[1] * vec3B.data[0],
            };
        }

        // Math equations
        VectorT operator+(const VectorT& vecB) const // Add
        {
            VectorT result; // Sum
            for (size_t i = 0; i < data.Length(); i++)
                result.data[i] = data[i] + vecB.data[i];
            return result;
        }
        VectorT operator-(const VectorT& vecB) const // Subtract
        {
            VectorT result; // Difference
            for (size_t i = 0; i < data.Length(); i++)
                result.data[i] = data[i] - vecB.data[i];
            return result;
        }

        VectorT operator*(const VectorT& vecB) const // Multiply
        {
            VectorT result; // Product
            for (size_t i = 0; i < data.Length(); i++)
                result.data[i] = data[i] * vecB.data[i];
            return result;
        }
        VectorT operator/(const VectorT& vecB) const // Divide
        {
            VectorT result; // Quotient
            for (size_t i = 0; i < data.Length(); i++)
                result.data[i] = data[i] / vecB.data[i];
            return result;
        }

        // Referencing
        FORCE_INLINE VT& operator[](size_t index) { return data[index]; }
        FORCE_INLINE const VT& operator[](size_t index) const { return data[index]; }
    public:
        CTL::Array<VT, C> data;
    };

    // Change this if you want Vectors to support
    // different types of variables by default
    using VectorVT = float32;

    template<size_t C>
    using Vector = VectorBase<C, VectorVT>;

    // TODO: do something about this 48 byte monstrosity
    class Vector4 : public Vector<4>
    {
    public:
        VectorVT& x = data[0];
        VectorVT& y = data[1];
        VectorVT& z = data[2];
        VectorVT& w = data[3];
    };
}