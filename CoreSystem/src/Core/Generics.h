/* Platform primitve type definitions */

#ifdef WIN32


// under Windows
#include <cstdint>


typedef int32_t int32;
typedef int64_t int64;

typedef uint32_t uint32;
typedef uint64_t uint64;

#else

// Unsupported Platform
typedef int int32
typedef int int64;

typedef unsigned int uint32;
typedef unsigned int uint64;



#endif