#pragma once
#include "EventSystem/Msvc/EventSystem.h"


namespace DatFrameworkEventSystem
{
	class IEvent;	

	/** 
	* Adapter-Interface class for all EventListeners which 
	* will be registered in an Eventmanager
	*/
	class EVENTSYSTEM_API IEventListener
	{
		// ============= Init ==============
	public:
		virtual ~IEventListener();	

		// ============= Interface Functions ==============
	public:
		/**
		 * This Function will be called if an Event which we listen to got triggered 
		 */
		virtual void EventCallback(IEvent* EventData) = 0;			

	protected:
		/**
		 * Cleans up any registration of that listener on an EventManager.
		 * Have to be called during deconstruction in a dtor to prevent memory leaks.
		 */
		void CleanupRegistration();
	};

	/**
	 * Abstract class where actual EventListeners will derive from
	 */
	class EVENTSYSTEM_API AbstractEventListener : public IEventListener
	{		
		// ============= Init ==============
	public:		
		virtual ~AbstractEventListener() {};

		virtual void EventCallback(IEvent* EventData) override = 0;
	};


}
