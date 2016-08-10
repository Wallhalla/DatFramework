#pragma once
#include "EventSystem/Msvc/EventSystem.h"

namespace DatFrameworkEventSystem
{	
	// forward declarations...	
	class IEvent;
	class IEventListener;	

	// ===========================================================
	// ========= IEventManager Interface =========================
	// ===========================================================
				
	/**
	 * Interface class visible to a client to communicate with a global eventmanager
	 */
	class EVENTSYSTEM_API IEventManager
	{
		friend class EventManagerFactory;
		friend class DefaultEventManager;

		// ========= Statics =======================
	public:
		/**
		 * Returns true if there is a valid Eventmanager available, false otherwise
		 */
		static bool DoesManagerExists();		

		/**
		 * Returns a reference to the global EventManager Instance
		 */
		static IEventManager& GetInstance();

		/**
		 * Entry to clean up any registrations and queues will call CleanUp of a Manager
		 */
		static void ClearInstance();

	private:
		/**
		* instance of the current GlobalEventManager
		*/
		static IEventManager* GlobalEventManager;

		// ========= Init =======================
	protected:
		IEventManager() = default;
		virtual ~IEventManager(){};

		// Disable Copy Ctor and Assignment
		IEventManager(const IEventManager&) = delete;
		IEventManager& operator=(const IEventManager&) = delete;

		// ========= Interface Methods ==========
	public:
		/**
		 * Registers a listener for a specific EventID 
		 */
		virtual bool RegisterListener(IEventListener* listener, const EventGUID& eventID) = 0;		
		/** 
		 * Unregisters a function of a Listener for a specific EventID 
		 */
		virtual bool UnregisterListener(IEventListener* listener, const EventGUID& eventID) = 0;
		/**
		* Unregisters a function of a Listener for a specific EventID
		*/
		virtual bool UnregisterListener(IEventListener* listener) = 0;
		/**
		 * Triggers all listeners, tries to process Event immediately
		 */
		virtual bool TriggerEvent(IEvent* const eventData) = 0;
		/** 
		 * Adds an Event to the queue, will be processed during next update
		 */
		virtual bool QueueEvent(IEvent* const eventData) = 0;
		/** 
		 * Ticks the queued Events, specify a timeout in miliseconds in case update loop can break after X miliseconds 
		 */
		virtual void TickQueue(unsigned long timeout = 0) = 0;				
		/** 
		 * Returns the name of this Manager 
		 */
		virtual char* GetName() const = 0;

	protected:	
		/**
		 * Entry to clean up manager owned memory 
		 */
		virtual void CleanUp() = 0;
		/**
		* Forwards event to all listeners
		*/
		virtual bool ForwardEvent(IEvent* const eventData) = 0;

		// ========= Methods ==========
	private:
		/**
		 * Used by EventManagerFactory only
		 */
		static void SetManager(IEventManager* manager);
		
	};

	/**
	 * Abstract Class an Implementation of an EventManager will derive from
	 */
	class EVENTSYSTEM_API AbstractEventManager : public IEventManager
	{
		// ========= Overrides =======================
	protected:		
		virtual void TickQueue(unsigned long timeout = 0) override = 0;	

	};

	// -------------- End IEventManager Interface --------------------
	

	// ===========================================================
	// ========= EventManager Factory =========================
	// ===========================================================

	/**
	 * Factory for EventManager instantiation
	 */
	class EVENTSYSTEM_API EventManagerFactory final
	{	

	public:
		/**
		 * Instantiated the given Managerclass. Template must inherit from IEventManager/AbstractEventManager
		 */
		template<class ManagerSubClass>
		static bool CreateManagerInstance()
		{			
			// fails if Manager doesnt derive from IEventManager -> Compiler error!
			CheckInheritance<ManagerSubClass, IEventManager>();

			if (!IEventManager::DoesManagerExists())
			{
				static ManagerSubClass Manager;

				IEventManager::SetManager(&Manager);

				return true;
			}

			return false;
		};		
	};

	// -------------- End EventManager Factory --------------------
}
