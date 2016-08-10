// Additional Includes
#include "../src/Core/Generics.h"
#include "../src/Core/Template.h"
#include "../src/Core/Macros.h"

//uncomment if you dont want to use Framework logging
//#define _DEBUG

#ifdef _DEBUG
// Enable(1)/Disable(0) logging
#define WITH_LOGGING 1
#include "../src/Core/Logging.h"
#else
#define WITH_LOGGING 0
#define LOG(str) { } 
#endif


