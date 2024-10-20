#include "CitromAssert.h"

#ifndef CT_OPTIMIZATION
namespace Citrom::Assertion
{
	void BreakApplication()
	{
		#ifdef CT_DEBUG
			DEBUG_BREAK();
		#elif defined(CT_RELEASE)
			// platform message box error "Assertion"
		#else // CT_OPTIMIZATION
		#endif
	}
}
#endif