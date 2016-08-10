#pragma once
#include "../EventManager.h"

// Standard Includes

#include <list>
#include <map>

// Maximum of lists with events
#define MAX_EVENT_QUEUES 2

namespace DatFrameworkEventSystem
{
	class IEvent;

	/**
	 * Default EventManager class, which handles event management
	 * via std library	
	 */
	class DefaultEventManagerImpl final : public AbstractEventManager
	{			
		friend class DefaultEventManager;		
			
		// ============= class typedefs ==============
		/* List of listeners */
		typedef std::list<IEventListener*> ListenerList;
		/* List of EventIDs */
		typedef std::list<EventGUID> EventIDList;
		/* Map of EventID's and listeners*/
		typedef std::map<EventGUID, ListenerList> EventIDListenersMap;
		/* Map of ListenerID and EventGUIDs */
		typedef std::map<IEventListener*, std::list<EventGUID>> ListenerEventsMap;
		/* List of Events */
		typedef std::list<IEvent*> EventQueue;

		// ============= Member ==============
	private:
		/* Key = EventGUID, Value = List of Listeners for this event */
		EventIDListenersMap EventMap;

		/* Key = Listener, Value = List of IDs */
		ListenerEventsMap ListenerMap;

		/* Eventqueues - lists of events */
		EventQueue Queues[MAX_EVENT_QUEUES];
		/* Index of the current active queue */
		uint32 ActiveQueueIndex;
	
		// ============= Init ==============
	private:
		DefaultEventManagerImpl();
		~DefaultEventManagerImpl();		

		// ============= Overrides ==============
	public:
		bool RegisterListener(IEventListener* listener, const EventGUID& eventID) override;		
		bool UnregisterListener(IEventListener* listener, const EventGUID& eventID) override;
		bool UnregisterListener(IEventListener* listener) override;
		bool QueueEvent(IEvent* const eventData) override;
		bool TriggerEvent(IEvent* const eventData) override;	
		void TickQueue(unsigned long timeout = 0) override;		
		char* GetName() const override;

	private:
		void CleanUp() override;
		bool ForwardEvent(IEvent* const eventData) override;

		// ============= Methods ==============
	private:
		bool AddListenerToEventMap(IEventListener* listener, const EventGUID& eventID);
		bool RemoveListenerFromEventMap(IEventListener* listener, const EventGUID& eventID);

		void AddIDToListener(IEventListener* listener, const EventGUID& eventID);
		void RemoveIDFromListener(IEventListener* listener, const EventGUID& eventID);
		
		void CleanEventMap();
		void CleanEventQueue();		
				
		/* checks if the ActiveQueueIndex is between 0 and max queues */
		bool IsQueueIndexValid() const;
				
	};	

	// -------------- End DefaultEventManagerImpl --------------------		
}
