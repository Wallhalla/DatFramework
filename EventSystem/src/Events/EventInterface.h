#pragma once
#include "EventSystem/Msvc/EventSystem.h"


#define INVALID_ACTOR_ID -1

namespace DatFrameworkEventSystem
{
	// ===========================================================
	// ============= Event Interface =============================
	// ===========================================================	

	/**
	* Interfaceclass for Event definition
	*/
	class EVENTSYSTEM_API IEvent
	{
		// ============= Statics ==============
	public:
		static bool CheckIfEventIsValidAndHasValidGUID(IEvent* eventData);

	private:
		static bool IsNotNull(IEvent* eventData);
		static bool HasValidGUID(IEvent* eventData);

		// ============= Init ==============
	public:		
		virtual ~IEvent();

		// ============= Interface Methods =============
	public:
		/**
		 * Returns the GUID for this Event 
		 */
		virtual const EventGUID& GetGUID() const = 0;
		/** 
		 * Returns the Event´s Name 
		 */
		virtual const char* GetName() const = 0;		
		/**
		 * Allocates a cloned Version of this event and returns it
		 */
		virtual IEvent* Clone() const= 0;
		
	};

	/**
	 * Baseclass each event derives from
	 */
	class EVENTSYSTEM_API AbstractBaseEvent : public IEvent
	{
		// ============= Init ==============
	public:		
		virtual ~AbstractBaseEvent();	
		
		// ============= Overrides ==============
	public:
		virtual const char* GetName() const override = 0;		
	};

	// -------------- End Event Interface --------------------

}