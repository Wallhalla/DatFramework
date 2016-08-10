#include "EventSystem/src/EventManager/EventManager.h"
#include "EventListenerInterface.h"

namespace DatFrameworkEventSystem
{
	// ---------------- IEventListener --------------------	

	IEventListener::~IEventListener()
	{
		LOG("Listener destroyed");
	}

	void IEventListener::CleanupRegistration()
	{	
		if (IEventManager::DoesManagerExists())
		{
			IEventManager& Manager = IEventManager::GetInstance();

			Manager.UnregisterListener(this);
		}		
	}

	// ---------------- AbstractEventListener --------------------	
}