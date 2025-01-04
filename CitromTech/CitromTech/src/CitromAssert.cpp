#include "CitromAssert.h"

#include "Platform/Platform.h"

#ifndef CT_OPTIMIZATION
namespace Citrom::Assertion
{
	void BreakApplication(const char* text)
	{
		#ifdef CT_DEBUG
			DEBUG_BREAK();
		#elif defined(CT_RELEASE)
			// platform message box error "Assertion"
			Platform::Utils::MessageBoxError("Assertion", text);
		#else // CT_OPTIMIZATION
		#endif
	}
}
#endif