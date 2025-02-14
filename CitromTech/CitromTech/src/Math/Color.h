#pragma once

#include "MathCommon.h"

namespace Citrom::Math
{
    // TODO: see if this templated shit is the equivalent of the past ones
    // or instead do not use templates and instead use macros to stream-line
    // the development of the other classes
    /*template<typename T, T AlphaDefault, T OtherDefault = 0> // cannot use float here as an argument in clang!!!
    class ColorX4
    {
    public:
        using ColorT = ColorX4<T, AlphaDefault, OtherDefault>;
    public:
        ColorX4(T red, T green, T blue, T alpha = AlphaDefault)
            : r(red), g(green), b(blue), a(alpha) {}
        ColorX4()
            : r(OtherDefault), g(OtherDefault), b(OtherDefault), a(OtherDefault) {}

        // Math equations
        ColorT operator+(const ColorT& colB) const { return ColorT{ static_cast<T>(this->r + colB.r), static_cast<T>(this->g + colB.g), static_cast<T>(this->b + colB.b), static_cast<T>(this->a + colB.a) }; } // Add
        ColorT operator-(const ColorT& colB) const { return ColorT{ static_cast<T>(this->r - colB.r), static_cast<T>(this->g - colB.g), static_cast<T>(this->b - colB.b), static_cast<T>(this->a - colB.a) }; } // Subtract

        ColorT operator*(const ColorT& colB) const { return ColorT{ static_cast<T>(this->r * colB.r), static_cast<T>(this->g * colB.g), static_cast<T>(this->b * colB.b), static_cast<T>(this->a * colB.a) }; } // Multiply
        ColorT operator/(const ColorT& colB) const { return ColorT{ static_cast<T>(this->r / colB.r), static_cast<T>(this->g / colB.g), static_cast<T>(this->b / colB.b), static_cast<T>(this->a / colB.a) }; } // Divide

        /*
         * Works because stack memory is stored contiguously,
         * No need to account for byte sizes (sizeof() operator) because if the type is known
         * the compiler already calculates it correctly
        *//*
        FORCE_INLINE T& operator[](size_t index) { return *(T*)(&r + index); }
        FORCE_INLINE const T& operator[](size_t index) const { return *(T*)(&r + index); }
    public:
        T r, g, b, a;
    };
    using ColorU8x4TemplatedTest = ColorX4<uint8, 0xFF, 0>;
    using ColorF32x4TemplatedTest = ColorX4<float32, 1.0f, 0.0f>;*/

	class ColorU8x4
	{
    public:
        ColorU8x4(uint8 red, uint8 green, uint8 blue, uint8 alpha = 0xFF)
            : r(red), g(green), b(blue), a(alpha) {}
        ColorU8x4()
            : r(0), g(0), b(0), a(0) {}

        // Math equations
        ColorU8x4 operator+(const ColorU8x4& colB) const { return ColorU8x4{ static_cast<uint8>(this->r + colB.r), static_cast<uint8>(this->g + colB.g), static_cast<uint8>(this->b + colB.b), static_cast<uint8>(this->a + colB.a) }; } // Add
        ColorU8x4 operator-(const ColorU8x4& colB) const { return ColorU8x4{ static_cast<uint8>(this->r - colB.r), static_cast<uint8>(this->g - colB.g), static_cast<uint8>(this->b - colB.b), static_cast<uint8>(this->a - colB.a) }; } // Subtract

        ColorU8x4 operator*(const ColorU8x4& colB) const { return ColorU8x4{ static_cast<uint8>(this->r * colB.r), static_cast<uint8>(this->g * colB.g), static_cast<uint8>(this->b * colB.b), static_cast<uint8>(this->a * colB.a) }; } // Multiply
        ColorU8x4 operator/(const ColorU8x4& colB) const { return ColorU8x4{ static_cast<uint8>(this->r / colB.r), static_cast<uint8>(this->g / colB.g), static_cast<uint8>(this->b / colB.b), static_cast<uint8>(this->a / colB.a) }; } // Divide

        /*
         * Works because stack memory is stored contiguously,
         * No need to account for byte sizes (sizeof() operator) because if the type is known
         * the compiler already calculates it correctly
        */
        FORCE_INLINE uint8& operator[](size_t index) { return *(uint8*)(&r + index); }
        FORCE_INLINE const uint8& operator[](size_t index) const { return *(uint8*)(&r + index); }
    public:
        uint8 r, g, b, a;
	};

	class ColorF32x3
	{
    public:
        ColorF32x3(float32 red, float32 green, float32 blue)
            : r(red), g(green), b(blue) {}
        ColorF32x3()
            : r(0.0f), g(0.0f), b(0.0f) {}

        // Math equations
        ColorF32x3 operator+(const ColorF32x3& colB) const { return ColorF32x3{ this->x + colB.x, this->y + colB.y, this->z + colB.z }; } // Add
        ColorF32x3 operator-(const ColorF32x3& colB) const { return ColorF32x3{ this->x - colB.x, this->y - colB.y, this->z - colB.z }; } // Subtract

        ColorF32x3 operator*(const ColorF32x3& colB) const { return ColorF32x3{ this->x * colB.x, this->y * colB.y, this->z * colB.z }; } // Multiply
        ColorF32x3 operator/(const ColorF32x3& colB) const { return ColorF32x3{ this->x / colB.x, this->y / colB.y, this->z / colB.z }; } // Divide

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
    class ColorF32x4
    {
    public:
        ColorF32x4(float32 red, float32 green, float32 blue, float32 alpha = 1.0f)
            : r(red), g(green), b(blue), a(alpha) {}
        ColorF32x4()
            : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}

        // Math equations
        ColorF32x4 operator+(const ColorF32x4& colB) const { return ColorF32x4{ this->r + colB.r, this->g + colB.g, this->b + colB.b, this->a + colB.a }; } // Add
        ColorF32x4 operator-(const ColorF32x4& colB) const { return ColorF32x4{ this->r - colB.r, this->g - colB.g, this->b - colB.b, this->a - colB.a }; } // Subtract

        ColorF32x4 operator*(const ColorF32x4& colB) const { return ColorF32x4{ this->r * colB.r, this->g * colB.g, this->b * colB.b, this->a * colB.a }; } // Multiply
        ColorF32x4 operator/(const ColorF32x4& colB) const { return ColorF32x4{ this->r / colB.r, this->g / colB.g, this->b / colB.b, this->a / colB.a }; } // Divide

        /*
         * Works because stack memory is stored contiguously,
         * No need to account for byte sizes (sizeof() operator) because if the type is known
         * the compiler already calculates it correctly
        */
        FORCE_INLINE float32& operator[](size_t index) { return *(float32*)(&r + index); }
        FORCE_INLINE const float32& operator[](size_t index) const { return *(float32*)(&r + index); }
    public:
        float32 r, g, b, a;
    };

    using Color = ColorF32x4;

    // TODO: finalize this
    inline ColorU8x4 HexCodeToColorU8x3(uint32 hexCode) // only the first 24 bytes are used
    {
        uint8 r = (hexCode >> 16) & 0xFF;
        uint8 g = (hexCode >> 8) & 0xFF;
        uint8 b = hexCode & 0xFF;

        return ColorU8x4(r, g, b);
    }
    inline ColorU8x4 HexCodeToColorU8x4(uint32 hexCode)
    {
        uint8 r = (hexCode >> 24) & 0xFF; // TODO: shouldn't all this be reversed?
        uint8 g = (hexCode >> 16) & 0xFF;
        uint8 b = (hexCode >> 8) & 0xFF;
        uint8 a = hexCode & 0xFF;

        return ColorU8x4(r, g, b, a);
    }

    FORCE_INLINE ColorU8x4 ColorF32x4ToColorU8x4(const ColorF32x4& color)
    {
        return ColorU8x4(FloatToUInt8Normalized(color.r), FloatToUInt8Normalized(color.g), FloatToUInt8Normalized(color.b), FloatToUInt8Normalized(color.a));
    }
    FORCE_INLINE ColorF32x4 ColorU8x4ToColorF32x4(const ColorU8x4& color)
    {
        return ColorF32x4(UInt8ToFloatNormalized(color.r), UInt8ToFloatNormalized(color.g), UInt8ToFloatNormalized(color.b), UInt8ToFloatNormalized(color.a));
    }

    //ColorF32x4 HSVToColorF32x3(const ColorHSV& color);
    //ColorF32x4 HSVToColorU8x4(const ColorHSV& color) {return ColorF32x4ToColorU8x4(HSVToColorF32x3(color))};
}