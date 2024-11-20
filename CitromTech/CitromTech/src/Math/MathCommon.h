#pragma once

#include "Core.h"

namespace Citrom::Math
{
	// l or L for long doubles, f or F for floats, nothing (can be represented as 0) for doubles.
	#define PI_MACRO(SUFFIX) 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420 ## SUFFIX
	constexpr floatl PI = PI_MACRO(l);

	#define POS_INFINITY_MACRO(SUFFIX) ( 1.0##SUFFIX / 0.0##SUFFIX)
	#define NEG_INFINITY_MACRO(SUFFIX) (-1.0##SUFFIX / 0.0##SUFFIX)
	#define NAN_MACRO(SUFFIX)		   (POS_INFINITY_MACRO(SUFFIX) * 0.0##SUFFIX)

	// Special checks for floating point numbers
	#define IS_NAN(x) ((x) != (x))
	#define IS_POSITIVELY_INFINITE(x) ((x) == POS_INFINITY_MACRO(f))
	#define IS_NEGATIVELY_INFINITE(x) ((x) == NEG_INFINITY_MACRO(f))

	#define IS_INFINITE(x) (IS_POSITIVELY_INFINITE(x) || IS_NEGATIVELY_INFINITE(x)) // ((x) == POS_INFINITY_MACRO(f) || (x) == NEG_INFINITY_MACRO(f))

	// TODO: MaxUInt8 vs UInt16Max
	constexpr uint8	 UInt8Max () { return 0xFF; }
	constexpr uint16 UInt16Max() { return 0xFFFF; }
	constexpr uint32 UInt32Max() { return 0xFFFFFFFF; }
	constexpr uint64 UInt64Max() { return 0xFFFFFFFFFFFFFFFF; }

	constexpr int8	 Int8Max  () { return 0x7F; }
	constexpr int16  Int16Max () { return 0x7FFF; }
	constexpr int32  Int32Max () { return 0x7FFFFFFF; }
	constexpr int64  Int64Max () { return 0x7FFFFFFFFFFFFFFF; }

	constexpr uint8	 UInt8Min () { return 0x00; }
	constexpr uint16 UInt16Min() { return 0x0000; }
	constexpr uint32 UInt32Min() { return 0x00000000; }
	constexpr uint64 UInt64Min() { return 0x0000000000000000; }

	constexpr int8   Int8Min  () { return static_cast<int8>(0x80); }
	constexpr int16  Int16Min () { return static_cast<int16>(0x8000); }
	constexpr int32  Int32Min () { return 0x80000000; }
	constexpr int64  Int64Min () { return 0x8000000000000000; }

	float32 SquareRoot(const float32 number);
	float64 SquareRoot(const float64 number);

	float32 InverseSquareRoot(const float32 number); // uses the Quake-3 style fast square root algorithm
	float64 InverseSquareRoot(const float64 number);
}