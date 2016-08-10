#pragma once
#include "EventSystem/Msvc/EventSystem.h"


namespace DatFrameworkEventSystem
{
	// ===========================================================
	// ========= DefaultEventManagerWrapper =============================
	// ===========================================================

	/**
	* DLL-Exposed Class which wraps the DefaultEventManagerImplementation
	*/
	class EVENTSYSTEM_API DefaultEventManager
	{
	public:
		/**
		 * Creates a DefaultManagerImplementation.
		 */
		static bool Create();		
	};

}

