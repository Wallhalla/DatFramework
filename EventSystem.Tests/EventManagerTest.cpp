#include "stdafx.h"
#include "CppUnitTest.h"

#include "EventSystem/src/EventManager/EventManager.h"
#include "EventSystem/src/EventManager/DefaultEventManager.h"
#include "EventSystem/src/EventListener/EventListenerInterface.h"
#include "EventSystem/src/Events/EventInterface.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace DatFrameworkEventSystem;

namespace EventSystemTests
{	
	/**
	 * Testclass to fake Listener objects
	 */
	class TestListener : public AbstractEventListener
	{
	public:
		TestListener()
		{
			
		};		

		virtual ~TestListener()
		{
			CleanupRegistration();
		};		
		
		void EventCallback(IEvent* pEvent) override
		{};		
	};

	/**
	* Testclass to fake Event objects
	*/
	class TestEvent : public AbstractBaseEvent
	{
	public:
		TestEvent(){};
		~TestEvent(){};

	private:
		static EventGUID eventID;

	public:
		void SetID(int value)
		{
			eventID = value;
		};

		virtual const char* GetName() const override
		{
			return "TestEvent";
		};

		virtual const EventGUID& GetGUID() const override
		{
			return eventID;
		};

		TestEvent* Clone() const override
		{
			return new TestEvent(*this);
		};
	};

	EventGUID TestEvent::eventID = 0;


	class TestManager : public AbstractEventManager
	{	
		friend class EventManagerFactory;

	public:
		TestManager(){};
		~TestManager()
		{
		};

	protected:
		bool RegisterListener(IEventListener* listener, const EventGUID& eventID) override
		{
			return false;
		};

		bool UnregisterListener(IEventListener* listener, const EventGUID& eventID) override
		{
			return false;
		};
		bool UnregisterListener(IEventListener* listener) override
		{
			return false;
		};
		bool TriggerEvent(IEvent* const eventData) override
		{
			return false;
		};
		bool QueueEvent(IEvent* const eventData) override
		{
			return false;
		};
		bool ForwardEvent(IEvent* const eventData) override
		{
			return false;
		};
		char* GetName() const override
		{
			return "TestManager";
		};

		void TickQueue(unsigned long timeout) override
		{};

	private:
		void CleanUp() override
		{

		};
	};

	TEST_CLASS(EventManagerInstanceTest)
	{
	public:

		/** 
		 * Creating an instance of an EventManager should not be null afterwards 
		 */
		TEST_METHOD(DefaultManager_CreateInstance_ShouldNotBeNull)
		{
			DefaultEventManager::Create();
			
			Assert::IsTrue(IEventManager::DoesManagerExists());			
		}

		/** 
		 * Creating multiple instances of same EventManager should fail 
		 */
		TEST_METHOD(DefaultManager_CreateMultipleInstance_ShouldBeFalse)
		{
			DefaultEventManager::Create();

			Assert::IsFalse(DefaultEventManager::Create());
		}

		/**
		* Creating a different instances of an EventManager should fail if there is already a valid manager
		*/
		TEST_METHOD(DefaultManager_CreateDifferentManager_ShouldBeFalse)
		{
			DefaultEventManager::Create();

			Assert::IsFalse(EventManagerFactory::CreateManagerInstance<TestManager>());
		}

		/**
		 * Switching the Eventmanager should be ok if the previous is released
		 */
		TEST_METHOD(EventManager_SwitchManager_ShouldBeTrue)
		{
			DefaultEventManager::Create();

			IEventManager::ClearInstance();

			Assert::IsTrue(EventManagerFactory::CreateManagerInstance<TestManager>());
		}

		/**
		 * Destroy the eventmanager. It should be null afterwards
		 */
		TEST_METHOD(EventManager_DestroyInstance_ShouldBeNull)
		{
			DefaultEventManager::Create();

			IEventManager::ClearInstance();

			Assert::IsNull(&IEventManager::GetInstance());
		}

		/**
		 * A pointer to the global eventmanager can be null
		 */
		TEST_METHOD(EventManager_GetUnInstantiatedManager_CanBeFalse)
		{			
			Assert::IsFalse(IEventManager::DoesManagerExists());			
		}

		/** 
		 * CleanUp Eventmanager instance
		 */
		TEST_METHOD_CLEANUP(methodName)
		{
			IEventManager::ClearInstance();
		}
	};

	TEST_CLASS(EventManagerMethodsTest)
	{
	public:	

		/**
		 * Before testing manager methods create a Manager for this test class
		 */

		TEST_CLASS_INITIALIZE(TestClassInit)
		{
			LOG("\n")
				LOG("======= Test Class Init =========")

			Assert::IsTrue(DefaultEventManager::Create());
			Assert::IsFalse(DefaultEventManager::Create());
		}

		TEST_METHOD_INITIALIZE(TestMethodInit)
		{
			LOG("\n")
			LOG("======= Test Method Init =========")
			
			LOG("\n")

			LOG("  Begin Test:")
		}	


		// ==============================
		// EventManager->RegisterListener
		// ==============================

		/**
		 * Registering an invalid listener(nullptr), but with a valid eventID, 
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_RegisterInvalidListener_ShouldReturnFalse)
		{
			LOG("	RegisterInvalidListener_ShouldReturnFalse")

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.RegisterListener(nullptr, 1));				
		}
		
		/**
		 * Registering a valid listener, but with an invalid eventID,
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_RegisterInvalidGUID_ShouldReturnFalse)
		{		
			LOG("	RegisterInvalidGUID_ShouldReturnFalse")			

			TestListener* listener = new TestListener();

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.RegisterListener(listener, 0));

			delete listener;
			listener = nullptr;
		}

		/**
		 * Registering a valid listener twice for the same eventID,
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_RegisterTwiceForSameEventID_ShouldReturnFalse)
		{
			LOG("	RegisterTwiceForSameEventID_ShouldReturnFalse")			

			TestListener* listener = new TestListener();
			
			IEventManager& manager = IEventManager::GetInstance();

			manager.RegisterListener(listener, 1);
			Assert::IsFalse(manager.RegisterListener(listener, 1));

			delete listener;
			listener = nullptr;
		}

		// ==============================
		// EventManager->UnregisterListener
		// ==============================

		/**
		 * Registering a valid listener for a valid eventID and deregister it again,
		 * should return true to the callee
		 */
		TEST_METHOD(EventManager_UnregisterRegisteredListener_ShouldReturnTrue)
		{
			LOG("	UnregisterRegisteredListener_ShouldReturnTrue")			
			
			TestListener* listener = new TestListener();

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsTrue(manager.RegisterListener(listener, 1));

			delete listener;
			listener = nullptr;
		}	

		/**
		 * Deregistering an invalid listener for a valid eventID,
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_UnregisterInvalidListener_ShouldReturnFalse)
		{
			LOG("	RegisterInvalidGUID_ShouldReturnFalse")			
			
			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.UnregisterListener(nullptr, 1));
		}

		/**
		* Deregistering a valid listener for an invalid eventID,
		* should return false to the callee
		*/
		TEST_METHOD(EventManager_UnregisterInvalidGUID_ShouldReturnFalse)
		{
			LOG("	UnregisterInvalidGUID_ShouldReturnFalse")
			
			TestListener* listener = new TestListener();

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.UnregisterListener(listener, 0));

			delete listener;
			listener = nullptr;			
		}

		// ==============================
		// EventManager->TriggerEvent
		// ==============================

		/**
		 * Trigger a valid event with registered listeners,
		 * should return true to the callee
		 */
		TEST_METHOD(EventManager_TriggerWithValidGUIDAndRegisteredListener_ShouldReturnTrue)
		{
			LOG("	TriggerWithValidGUIDAndRegisteredListener_ShouldReturnTrue")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(1);

			TestListener* listener = new TestListener();
			
			IEventManager& manager = IEventManager::GetInstance();
			
			manager.RegisterListener(listener, pEvent->GetGUID());

			Assert::IsTrue(manager.TriggerEvent(pEvent));

			delete pEvent;
			pEvent = nullptr;

			delete listener;
			listener = nullptr;
		}

		/**
		 * Trigger an invalid event,
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_TriggerInvalidEvent_ShouldReturnFalse)
		{
			LOG("	TriggerInvalidEvent_ShouldReturnFalse")
			
			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.TriggerEvent(nullptr));				
		}

		/**
		 * Trigger a valid event but with invalid GUID,
		 * should return false to the callee
		 */
		TEST_METHOD(EventManager_TriggerWithInvalidGUID_ShouldReturnFalse)
		{
			LOG("	TriggerWithInvalidGUID_ShouldReturnFalse")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(0);
			
			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.TriggerEvent(pEvent));

			delete pEvent;
			pEvent = nullptr;
		}

		/**
		* Trigger valid event for a registered Listener, which was destroyed
		* should return false to the callee
		*/
		TEST_METHOD(EventManager_TriggerEvent_ShouldBeOk_WithDestroyedListener)
		{
			LOG("	TriggerEvent_ShouldBeOk_WithDestroyedListener")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(1);

			TestListener* listener = new TestListener();

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsTrue(manager.RegisterListener(listener, pEvent->GetGUID()));

			delete listener;
			listener = nullptr;

			Assert::IsFalse(manager.TriggerEvent(pEvent));

			delete pEvent;
			pEvent = nullptr;
			
		}

		// ==============================
		// EventManager->QueueEvent
		// ==============================
		
		/**
		* Queue a valid event,
		* should return true to the callee 
		*/
		TEST_METHOD(EventManager_QueueWithValidGUID_ShouldReturnTrue)
		{
			LOG("	QueueWithValidGUID_ShouldReturnTrue")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(1);			

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsTrue(manager.QueueEvent(pEvent));
			
			delete pEvent;
			pEvent = nullptr;	
			
		}

		/**
		* Queue an invalid event,
		* should return false to the callee
		*/
		TEST_METHOD(EventManager_QueueInvalidEvent_ShouldReturnFalse)
		{
			LOG("	QueueInvalidEvent_ShouldReturnFalse")
			
			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.QueueEvent(nullptr));
		}

		/**
		* Queue a valid event but with invalid GUID,
		* should return false to the callee
		*/
		TEST_METHOD(EventManager_QueueWithInvalidGUID_ShouldReturnFalse)
		{
			LOG("	QueueWithInvalidGUID_ShouldReturnFalse")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(0);

			IEventManager& manager = IEventManager::GetInstance();

			Assert::IsFalse(manager.QueueEvent(pEvent));

			delete pEvent;
			pEvent = nullptr;
		}

		// ==============================
		// EventManager->TickQueue
		// ==============================
		TEST_METHOD(EventManager_TickQueue_ShouldBeOK_WithNULLTimeoutTime)
		{
			LOG("	TickQueue_ShouldBeOK_WithNULLTimeoutTime")
			
			IEventManager& manager = IEventManager::GetInstance();

			manager.TickQueue();
			
		}

		TEST_METHOD(EventManager_TickQueue_ShouldBeOK_WithTimeoutTime)
		{
			LOG("	TickQueue_ShouldBeOK_WithTimeoutTime")

			IEventManager& manager = IEventManager::GetInstance();

			manager.TickQueue(30);
		}

		TEST_METHOD(EventManager_TickQueue_ShouldBeOK_WithNegativeTimeoutTime)
		{
			LOG("	TickQueue_ShouldBeOK_WithNegativeTimeoutTime")
			
			IEventManager& manager = IEventManager::GetInstance();

			manager.TickQueue(-30);
		}

		TEST_METHOD(EventManager_TickQueue_ShouldBeOK_WithDestroyedEvent)
		{
			LOG("	TickQueue_ShouldBeOK_WithDestroyedEvent")
			
			TestEvent* pEvent = new TestEvent();

			pEvent->SetID(1);

			TestListener* listener = new TestListener();

			IEventManager& manager = IEventManager::GetInstance();

			manager.RegisterListener(listener, pEvent->GetGUID());

			Assert::IsTrue(manager.QueueEvent(pEvent));

			delete pEvent;
			pEvent = nullptr;			

			delete listener;
			listener = nullptr;

			manager.TickQueue();
		}

		TEST_METHOD(Event_CloneEvent_ShouldNotBeTheSame)
		{
			TestEvent* pEvent = new TestEvent();

			IEvent* copy = pEvent->Clone();

			bool areEqual = false;

			if (pEvent == copy)
			{
				areEqual = true;
			}

			Assert::IsFalse(areEqual);

			delete pEvent;
			pEvent = nullptr;

			delete copy;
			copy = nullptr;
		}

		// ==============================
		// CleanUp after each TestMethod
		// ==============================
		TEST_METHOD_CLEANUP(MethodCleanup)
		{			
			LOG("\n")
			LOG("========= Test Method Cleanup ========")						
			LOG("\n")
		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			LOG("\n")
			LOG("========= Test Class Cleanup ========")
			IEventManager::ClearInstance();
			LOG("\n")
		}
};
}