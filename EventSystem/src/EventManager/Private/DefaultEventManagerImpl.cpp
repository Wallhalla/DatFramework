#include "DefaultEventManagerImpl.h"
#include "../../Events/EventInterface.h"
#include "../../EventListener/EventListenerInterface.h"

// needed to get access to platform tick
#include "CoreSystem/src/Core/Platform.h"

namespace DatFrameworkEventSystem
{
	
	// ===========================================================
	// ========= DefaultEventManagerImpl =============================
	// ===========================================================	

	DefaultEventManagerImpl::DefaultEventManagerImpl()		
	{
		EventMap.clear();
		ListenerMap.clear();
		ActiveQueueIndex = 0;

		LOG((std::string(this->GetName()) + std::string(": instantiated!")).c_str())
	}

	DefaultEventManagerImpl::~DefaultEventManagerImpl()
	{
		CleanUp();
	}

	void DefaultEventManagerImpl::CleanUp()
	{
		CleanEventMap();
		CleanEventQueue();
	}

	void DefaultEventManagerImpl::CleanEventMap()
	{
		for (EventIDListenersMap::iterator it = EventMap.begin(); it != EventMap.end(); ++it)
		{
			ListenerList listeners = EventMap[it->first];
			
			while (!listeners.empty())
			{
				UnregisterListener(listeners.front(), it->first);

				listeners.pop_front();
			}

			listeners.clear();
		}

		EventMap.clear();
		ListenerMap.clear();
	}

	void DefaultEventManagerImpl::CleanEventQueue()
	{
		for (int i = 0; i < MAX_EVENT_QUEUES; i++)
		{
			if (!Queues[i].empty())
			{
				EventQueue queue = Queues[i];

				while (!queue.empty())
				{
					delete queue.back();

					queue.pop_back();
				}
			}
		}
	}

	bool DefaultEventManagerImpl::RegisterListener(IEventListener* listener, const EventGUID& eventID)
	{
		bool success = false;

		if (listener && eventID > 0)
		{
			success = AddListenerToEventMap(listener, eventID);
		}
		else
		{
			LOG("Listener or EventID are invalid")
		}

		return success;
	}

	bool DefaultEventManagerImpl::UnregisterListener(IEventListener* listener, const EventGUID& eventID)
	{
		bool success = false;

		if (listener && eventID > 0)
		{
			success = RemoveListenerFromEventMap(listener, eventID);			
		}
		else
		{
			LOG("Listener or EventID is invalid")
		}

		return success;
	}

	bool DefaultEventManagerImpl::UnregisterListener(IEventListener* listener)
	{
		bool success = false;

		if (listener)
		{
			EventIDList registeredIDs = ListenerMap[listener];

			for (auto it = registeredIDs.begin(); it != registeredIDs.end(); ++it)
			{
				EventGUID ID = (*it);
				RemoveListenerFromEventMap(listener, ID);
			}

			LOG("Listener was unregistered completely")
			success = true;
		}
		else
		{
			LOG("Listener is invalid!")
		}

		return success;
	}

	bool DefaultEventManagerImpl::QueueEvent(IEvent* const eventData)
	{
		bool eventQueued = false;

		if (IEvent::CheckIfEventIsValidAndHasValidGUID(eventData))
		{
			if (IsQueueIndexValid())
			{
				IEvent* copy = eventData->Clone();

				// Add event to queue
				Queues[ActiveQueueIndex].push_back(copy);
				eventQueued = true;

				LOG("Event queued")
			}
		}

		return eventQueued;
	}


	bool DefaultEventManagerImpl::TriggerEvent(IEvent* const eventData)
	{
		bool processed = false;

		if (IEvent::CheckIfEventIsValidAndHasValidGUID(eventData))
		{
			processed = ForwardEvent(eventData);
		}

		return processed;
	}

	void DefaultEventManagerImpl::TickQueue(unsigned long timeout)
	{
		unsigned long MaxMiliseconds = 0;

		if (timeout > 0)
		{
			unsigned long CurrentMiliseconds = Platform::GetPlatformTickCount();
			MaxMiliseconds = timeout + CurrentMiliseconds;
		}

		uint32 CurrentQueue = ActiveQueueIndex;

		// swap queues modulo to stay in range of Max_Event_Queues
		ActiveQueueIndex = (ActiveQueueIndex + 1) % MAX_EVENT_QUEUES;
		Queues[ActiveQueueIndex].clear();

		IEvent* eventPtr = nullptr;
		// Process current queue
		while (!Queues[CurrentQueue].empty())
		{
			LOG(std::to_string(Queues[CurrentQueue].size()).c_str());
			// take the first event in queue...
			eventPtr = Queues[CurrentQueue].front();

			// ... remove event from list ...
			Queues[CurrentQueue].pop_front();

			// ... process it ...
			ForwardEvent(eventPtr);

			delete eventPtr;
			eventPtr = nullptr;

			// check if Update time has ran out
			if (Platform::GetPlatformTickCount() > MaxMiliseconds)
			{
				// current TickCount expired the maximum time for updating -> break
				break;
			}
		}

		// check if the current queue is not empty
		bool QueueNotEmpty = Queues[CurrentQueue].empty();
		if (!QueueNotEmpty)
		{
			// insert all remaining events into the new active queue and
			// remove them from the current one
			while (!Queues[CurrentQueue].empty())
			{
				auto queuedEvent = Queues[CurrentQueue].back();

				Queues[CurrentQueue].pop_back();

				Queues[ActiveQueueIndex].push_front(queuedEvent);
			}
		}
	}

	bool DefaultEventManagerImpl::AddListenerToEventMap(IEventListener* listener, const EventGUID& eventID)
	{
		ListenerList& eventListenerList = EventMap[eventID];  // this will find or create the entry

		for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
		{
			if (listener == (*it))
			{
				LOG("Listener tries to register twice for the same event")
				return false;
			}
		}

		eventListenerList.push_back(listener);

		AddIDToListener(listener, eventID);

		LOG("Listener was registered")

		return true;
	}

	bool DefaultEventManagerImpl::RemoveListenerFromEventMap(IEventListener* listener, const EventGUID& eventID)
	{
		bool success = false;

		EventIDListenersMap::iterator findIt = EventMap.find(eventID);
		if (findIt != EventMap.end())
		{
			ListenerList& listeners = findIt->second;
			for (auto it = listeners.begin(); it != listeners.end(); ++it)
			{
				if (listener == (*it))
				{
					RemoveIDFromListener(listener, eventID);

					listeners.erase(it);

					LOG("Listener was unregistered for Event")

					success = true;
					return success;
				}
			}

			if (listeners.empty())
			{
				EventMap.erase(eventID);
			}
		}

		if (!success)
		{
			LOG("No Listener found to unregister")
		}

		return success;
	}

	void DefaultEventManagerImpl::AddIDToListener(IEventListener* listener, const EventGUID& eventID)
	{
		EventIDList& idList = ListenerMap[listener];

		idList.push_back(eventID);
	}

	void DefaultEventManagerImpl::RemoveIDFromListener(IEventListener* listener, const EventGUID& eventID)
	{
		ListenerEventsMap::iterator findIt = ListenerMap.find(listener);

		if (findIt != ListenerMap.end())
		{
			EventIDList& idList = findIt->second;
			for (auto it = idList.begin(); it != idList.end(); ++it)
			{
				if (eventID == (*it))
				{
					idList.erase(it);
					return;
				}
			}

			if (idList.empty())
			{
				ListenerMap.erase(listener);
			}
		}

	}

	bool DefaultEventManagerImpl::ForwardEvent(IEvent* const eventData)
	{
		bool wasForwarded = false;

		auto findIt = EventMap.find(eventData->GetGUID());
		if (findIt != EventMap.end() && !findIt->second.empty())
		{
			LOG("ForwardEvent: There are Listeners!")

			const ListenerList& eventListenerList = findIt->second;
			for (ListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
			{
				IEventListener* listener = (*it);	
				
				listener->EventCallback(eventData);

				LOG("Event forwarded")

				wasForwarded = true;
			}
		}
		else
		{
			LOG("ForwardEvent: no Listeners found!")
		}		

		return wasForwarded;
	}

	char* DefaultEventManagerImpl::GetName() const
	{
		return "DefaultEventManagerImpl";
	}	

	bool DefaultEventManagerImpl::IsQueueIndexValid() const
	{
		bool output = false;
		// Queue index needs to be between 0 and MAX_EVENT_QUEUES
		if (ActiveQueueIndex >= 0 && ActiveQueueIndex < MAX_EVENT_QUEUES)
		{
			output = true;
		}

		return output;
	}

	// -------------- End DefaultEventManagerImpl --------------------
}
