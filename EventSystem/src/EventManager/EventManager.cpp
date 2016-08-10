#include "EventManager.h"
#include "../Events/EventInterface.h"


namespace DatFrameworkEventSystem
{
	// ===========================================================
	// ========= IEventManager Interface =========================
	// ===========================================================

	// Statics
	IEventManager* IEventManager::GlobalEventManager = NULL;
	
		
	bool IEventManager::DoesManagerExists()
	{
		return (GlobalEventManager != NULL);
	}	

	IEventManager& IEventManager::GetInstance()
	{
		return *GlobalEventManager;
	}

	void IEventManager::ClearInstance()
	{
		if (GlobalEventManager != NULL)
		{	
			GlobalEventManager->CleanUp();
			GlobalEventManager = NULL;
		}
	}

	void IEventManager::SetManager(IEventManager* manager)
	{
		if (GlobalEventManager != NULL)
		{
			LOG("There is already a valid EventManager available! Please destroy it first")
		}
		else
		{
			GlobalEventManager = manager;
		}
	}
	

	// -------------- End IEventManager Interface --------------------	
}