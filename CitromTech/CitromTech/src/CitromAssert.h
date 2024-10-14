#pragma once

// TODO: everything

#include "Core.h"

#ifdef CT_DEBUG
#define CT_CORE_ASSERT(x, y) if (!(x)) {logger_critical("CT2D: ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr}", __FILE__, __LINE__, #y, #x); DEBUG_BREAK();}
#define CT_MOD_ASSERT(x, y) if (!(x)) {logger_critical("MODULE: ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr}", __FILE__, __LINE__, #y, #x); DEBUG_BREAK();}
#define CT_ASSERT(x, y) if (!(x)) {logger_critical("ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr} in function: {cstr}()", __FILE__, __LINE__, #y, #x, __func__); DEBUG_BREAK();}
#define CT_ASSERT_WARN(x, y) if (!(x)) {CT_WARN(y);}
#elif defined(CT_RELEASE)
#define CT_CORE_ASSERT(x, y) if (!(x)) {logger_critical("CT2D: ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr}", __FILE__, __LINE__, #y, #x); platform_message_box_error("Assertion", y);}
#define CT_MOD_ASSERT(x, y) if (!(x)) {logger_critical("MODULE: ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr}", __FILE__, __LINE__, #y, #x); platform_message_box_error("Module Assertion", y);}
#define CT_ASSERT(x, y) if (!(x)) {logger_critical("ASSERT: ", "{cstr}({int}): error: {cstr} line: {cstr} in function: {cstr}()", __FILE__, __LINE__, #y, #x, __func__); platform_message_box_error("Assertion", y);}
#define CT_ASSERT_WARN(x, y) if (!(x)) {CT_WARN(y);}
#else // CT_OPTIMIZATION
#define CT_CORE_ASSERT(x, y) x
#define CT_MOD_ASSERT(x, y) x
#define CT_ASSERT(x, y) x
#define CT_ASSERT_WARN(x, y) x
#endif