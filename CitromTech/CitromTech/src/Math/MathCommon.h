#pragma once

#include "Core.h"

namespace Citrom::Math
{
	// l or L for long doubles, f or F for floats, nothing (can be represented as 0) for doubles.
	#define PI_MACRO(SUFFIX) 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420 ## SUFFIX
	constexpr float64 PI = PI_MACRO(0);
	constexpr float32 PIf = PI_MACRO(f);
	constexpr floatl PIl = PI_MACRO(l);

	#define POS_INFINITY_MACRO(SUFFIX) ( 1.0##SUFFIX / 0.0##SUFFIX)
	#define NEG_INFINITY_MACRO(SUFFIX) (-1.0##SUFFIX / 0.0##SUFFIX)
	#define NAN_MACRO(SUFFIX)		   (POS_INFINITY_MACRO(SUFFIX) * 0.0##SUFFIX)

	#define WHOLEF_SUFFIXED(WHOLENUMBER, SUFFIX) WHOLENUMBER ## .0 ## SUFFIX
	#define WHFSUF(WHOLENUMLITERAL, SUFFIX) WHOLEF_SUFFIXED(WHOLENUMLITERAL, SUFFIX) // WHFLTSUF, WHLFSUFF, FLTSUFF, WFLTSFX, FL_SFX, WHFSUF, WH_F_SUF, WHF_SUF

	// Special checks for floating point numbers
	#define IS_NAN(x) ((x) != (x))
	#define IS_POSITIVELY_INFINITE(x) ((x) == POS_INFINITY_MACRO(f))
	#define IS_NEGATIVELY_INFINITE(x) ((x) == NEG_INFINITY_MACRO(f))

	#define IS_INFINITE(x) (IS_POSITIVELY_INFINITE(x) || IS_NEGATIVELY_INFINITE(x)) // ((x) == POS_INFINITY_MACRO(f) || (x) == NEG_INFINITY_MACRO(f))

	#define IS_BETWEEN(value, min, max) ((value) >= (min) && (value) <= (max))

	#define SQUARED(x) ((x) * (x))

	// Rotation
	#define RADIANS_TO_DEGREES(RADIANS, SUFFIX) ((RADIANS) * (180.0 ## SUFFIX / PI_MACRO(SUFFIX)))
	#define DEGREES_TO_RADIANS(DEGREES, SUFFIX) ((DEGREES) * (PI_MACRO(SUFFIX) / 180.0 ## SUFFIX))

	// Radians
	#define RADIANS_TO_ARCSECONDS(RADIANS, SUFFIX)		((RADIANS) * ((WHFSUF(3600, SUFFIX) * WHFSUF(180, SUFFIX)) / PI_MACRO(SUFFIX)))		// rad * (3600 * 180) / PI
	#define RADIANS_TO_GRADIANS(RADIANS, SUFFIX)		((RADIANS) * (WHFSUF(200, SUFFIX) / PI_MACRO(SUFFIX)))								// rad * 200/PI
	#define RADIANS_TO_MILLIRADIANS(RADIANS, SUFFIX)	((RADIANS) * WHFSUF(1000, SUFFIX))													// rad * 1000
	#define RADIANS_TO_MINUTESOFARC(RADIANS, SUFFIX)	((RADIANS) * ((WHFSUF(60, SUFFIX) * WHFSUF(180, SUFFIX)) / PI_MACRO(SUFFIX)))		// rad * (60 * 180) / PI
	//#define RADIANS_TO_MINUTESOFARC(RADIANS, SUFFIX)	((RADIANS) * (1))		// rad * (60 * 180) / PI

	// Degrees
	#define DEGREES_TO_ARCSECONDS(DEGREES, SUFFIX)		((DEGREES) * 3600.0 ## SUFFIX)													// o * 3600
	#define DEGREES_TO_GRADIANS(DEGREES, SUFFIX)		((DEGREES) * (WHFSUF(200, SUFFIX) / WHFSUF(180, SUFFIX)))						// o * 200/180
	#define DEGREES_TO_MILLIRADIANS(DEGREES, SUFFIX)	((DEGREES) * ((WHFSUF(1000, SUFFIX) * PI_MACRO(SUFFIX)) / WHFSUF(180, SUFFIX))) // o * 1000PI/180
	#define DEGREES_TO_MINUTESOFARC(DEGREES, SUFFIX)	((DEGREES) * WHFSUF(60, SUFFIX))												// o * 60
	//#define DEGREES_TO_MINUTESOFARC(DEGREES, SUFFIX)	((DEGREES) * (1)) // o * 60

	// Arcseconds // Important for space stuff
	// deg = arcsec / 3600
	// grad = arcsec / 3240
	// mrad = arcsec * 1000PI / (180 * 3600)
	// arcmin = arcsec / 60
	// rad = arcsec * PI / (180 * 3600)

	// Gradians // Maybe Important for engineering?
	// deg = grad * 180/200
	// rad = grad * PI/200
	// arcsec = grad * 3240
	// mrad = grad * 1000PI/200
	// arcmin = grad * 54

	// Milliradians // Important for military stuff
	// deg = mrad * 180/1000PI
	// rad = mrad / 1000
	// arcsec = mrad * (3600 * 180) / 1000PI
	// grad = mrad * 200/1000PI
	// arcmin = mrad * (60 * 180) / 1000PI

	// Minutes of Arc // Important for space stuff
	// deg = arcmin / 60
	// rad = arcmin * PI / (60 * 180)
	// arcsec = arcmin * 60
	// grad = arcmin / 54
	// mrad = arcmin * 1000PI / (60 * 180)

    #include "float.h"

    #define FLOAT32_MAX (FLT_MAX)
    #define FLOAT64_MAX (DBL_MAX)
    #define FLOATL_MAX  (LDBL_MAX)

	// TODO: MaxUInt8 vs UInt16Max
	constexpr uint8	 UInt8Max () { return 0xFF; }
	constexpr uint16 UInt16Max() { return 0xFFFF; }
	constexpr uint32 UInt32Max() { return 0xFFFFFFFF; }
	constexpr uint64 UInt64Max() { return 0xFFFFFFFFFFFFFFFF; }

	constexpr int8	 Int8Max  () { return SCast<int8>(0x7F); }
	constexpr int16  Int16Max () { return SCast<int16>(0x7FFF); }
	constexpr int32  Int32Max () { return SCast<int32>(0x7FFFFFFF); }
	constexpr int64  Int64Max () { return SCast<int64>(0x7FFFFFFFFFFFFFFF); }

	constexpr uint8	 UInt8Min () { return 0x00; }
	constexpr uint16 UInt16Min() { return 0x0000; }
	constexpr uint32 UInt32Min() { return 0x00000000; }
	constexpr uint64 UInt64Min() { return 0x0000000000000000; }

	constexpr int8   Int8Min  () { return SCast<int8>(0x80); }
	constexpr int16  Int16Min () { return SCast<int16>(0x8000); }
	constexpr int32  Int32Min () { return SCast<int32>(0x80000000); }
	constexpr int64  Int64Min () { return SCast<int64>(0x8000000000000000); }

	constexpr float32 UInt8ToFloatNormalized(uint8 value) { return value / (float32)UInt8Max(); }
	constexpr uint8 FloatToUInt8Normalized(float32 value) { return (uint8)value * UInt8Max(); }

    constexpr float32 Float32Max() { return FLOAT32_MAX; }

	float32 SquareRoot(const float32 number);
	float64 SquareRoot(const float64 number);

	float32 Sine(const float32 number);
	float64 Sine(const float64 number);

	float32 Cosine(const float32 number);
	float64 Cosine(const float64 number);

	float32 InverseSquareRoot(const float32 number); // uses the Quake-3 style fast square root algorithm
	float64 InverseSquareRoot(const float64 number);

	float32 FovXToFovY(float32 fovX, float32 aspect);
	float32 FovYToFovX(float32 fovY, float32 aspect);

	constexpr float32 DegreesToRadians(float32 degrees)
	{
		return DEGREES_TO_RADIANS(degrees, f);
	}
	constexpr float64 DegreesToRadians(float64 degrees)
	{
		return DEGREES_TO_RADIANS(degrees, 0);
	}
	constexpr float32 RadiansToDegrees(float32 radians)
	{
		return RADIANS_TO_DEGREES(radians, f);
	}
	constexpr float64 RadiansToDegrees(float64 radians)
	{
		return RADIANS_TO_DEGREES(radians, 0);
	}

	float32 Clamp(float32 value, float32 min, float32 max);

	// Fun stuff
	namespace AccuratePI
	{
		float64 Leibzniz(uint32fast terms = 1000000); // Simple and Slow
		float64 Machin();
		//float64 Chudnovsky(mpfr_t result, int digits); // Faster but more complex
		float64 MonteCarlo(uint32fast samples = 1000000); // Random
	}
}
