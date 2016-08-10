#include "DefaultEventManager.h"

#include "Private/DefaultEventManagerImpl.h"

using namespace DatFrameworkEventSystem;

bool DefaultEventManager::Create()
{
	if (!IEventManager::DoesManagerExists())
	{
		static DefaultEventManagerImpl Manager;

		IEventManager::SetManager(&Manager);		

		return true;
	}

	return false;
};