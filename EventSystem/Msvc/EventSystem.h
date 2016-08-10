#ifdef  EVENTSYSTEM_EXPORTS
#define EVENTSYSTEM_API __declspec(dllexport)
#else
#define EVENTSYSTEM_API __declspec(dllimport)
#endif

#include "EventSystemStd.h"
