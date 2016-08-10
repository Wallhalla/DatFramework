
/*

This file wraps platformcalls of the used platform the client develops on

*/

namespace Platform
{

	// Using Windows
#ifdef WIN32

#include "windows.h"

	static unsigned long GetPlatformTickCount()
	{
		return GetTickCount();
	};


#else

	// Unsupported Platform
	static unsigned long GetPlatformTickCount()
	{
		return 0;
	};


#endif
}

