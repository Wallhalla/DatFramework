#include "EventInterface.h"

namespace DatFrameworkEventSystem
{
	// ===========================================================
	// ============= Event Interface =============================
	// ===========================================================

	IEvent::~IEvent()
	{
		// empty
	}

	bool IEvent::CheckIfEventIsValidAndHasValidGUID(IEvent* eventData)
	{
		return (IsNotNull(eventData) && HasValidGUID(eventData));
	}

	bool IEvent::IsNotNull(IEvent* eventData)
	{
		bool isValid = false;
		if (eventData)
		{
			isValid = true;
			
		}
		else
		{
			LOG("Event is invalid")
		}

		return isValid;
	}

	bool IEvent::HasValidGUID(IEvent* eventData)
	{
		bool isValid = false;

		if (eventData->GetGUID() > 0)
		{
			// TODO: further checks

			isValid = true;
		}
		else
		{
			LOG((std::string(eventData->GetName()) + std::string(": has an invalid ID")).c_str())
		}

		return isValid;
	}
	
	AbstractBaseEvent::~AbstractBaseEvent()
	{
		// empty
	}

	

	// -------------- End Event Interface --------------------
}
