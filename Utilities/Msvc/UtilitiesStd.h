#pragma once

#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif

#include "CoreSystem/Msvc/CoreSystem.h"


#define INVALID_GUID 0


// Namespace descriptions for Doxygen

/**
 * SubNamespace of Utilities, all algorithm related utils
 * will be nested inside this namespace
 */
namespace DatFrameworkAlgorithms
{
	/**
	 * Algorithm namespace for generating GUID´s
	 */
	namespace GuidGenerator
	{
	}
}