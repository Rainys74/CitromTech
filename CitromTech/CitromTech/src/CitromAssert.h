#pragma once

#include "Core.h"
#include "Logger/Logger.h"

#ifdef CT_DEBUG
#define CT_ASSERT_THROW_APP_ERROR() DEBUG_BREAK()
#else
#define CT_ASSERT_THROW_APP_ERROR() Citrom::Assertion::BreakApplication()
#endif

#if defined(CT_DEBUG) || defined(CT_RELEASE)
#define CT_CORE_ASSERT(x, y) if (!(x)) {CT_CORE_FATAL("ASSERT: ""{}({}): error: {} line: {} in function: {}()", __FILE__, __LINE__, #y, #x, __func__); CT_ASSERT_THROW_APP_ERROR();}
#define CT_ASSERT(x, y) if (!(x)) {CT_FATAL("ASSERT: ""{}({}): error: {} line: {} in function: {}()", __FILE__, __LINE__, #y, #x, __func__); CT_ASSERT_THROW_APP_ERROR();}
#define CT_CORE_ASSERT_WARN(x, y) if (!(x)) {CT_CORE_WARN("WARNING ASSERT: ""{}({}): error: {} line: {} in function: {}()", __FILE__, __LINE__, #y, #x, __func__); }

#define CT_CORE_VERIFY(x, y) CT_CORE_ASSERT(x, y)
#define CT_VERIFY(x, y) CT_ASSERT(x, y)
#else // CT_OPTIMIZATION
#define CT_CORE_ASSERT(x, y) x// ## throwerror
#define CT_ASSERT(x, y) x// ## throwerror
#define CT_CORE_ASSERT_WARN(x, y) x

#define CT_CORE_VERIFY(x, y) x
#define CT_VERIFY(x, y) x
#endif

#ifndef CT_OPTIMIZATION
namespace Citrom::Assertion
{
	// this exists so that there's no need for including platform code
	// on release builds to use the message box errors
	void BreakApplication();
}
#endif