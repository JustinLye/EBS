#ifndef BASE_MODULE_CC_INCLUDED
#define BASE_MODULE_CC_INCLUDED
#include"ebs/BaseModule.h"
#ifdef BUILD_BASE_EVENT_MODULE_CC

unsigned int BaseModule::NextModuleId = 0;



//////////////////////////////////////////////////////////////////////
///\ fn EntryPoint()
///\ brief Entry point when thread is launched
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void BaseModule::EntryPoint()
	{
		if (!LookupEventHandler(MakeEventPtr(name_t::SHUTDOWNEVENT)))
		{
			RegisterEventHandler(name_t::SHUTDOWNEVENT, &BaseModule::HandleShutdown);
		}
		EventLoop();
	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::GetMail()
///\ brief Entry point when thread is launched
///\ returns Next message in mailbox queue
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	BaseModule::bound_cb BaseModule::GetMail()
	{
		return mMailBox.GetMail();
	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::EventLoop()
///\ brief Loop that runs until mShutdown is set to true
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


		void BaseModule::EventLoop()
		{
			{
				std::lock_guard<std::mutex> locker(mUpdateStateMtx);
				mModuleState = MODULE_IS_READY;
				mUpdateStateCond.notify_all();
			}
			while (!mShutdown)
			{
				bound_cb handler = mMailBox.GetMail();
				(*handler.second)(handler.first);
			}
		}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::HandleShutdown(BaseModule::event_ptr)
///\ brief Loop that runs until mShutdown is set to true
///\ param[in] event_item is a pointer to a BaseEvent w/ name SHUTDOWN
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void BaseModule::HandleShutdown(BaseModule::event_ptr event_item)
	{
		{
			std::lock_guard<std::mutex> locker(mUpdateStateMtx);
			mModuleState = MODULE_NOT_READY;
		}
		std::cout << "Shutting down. Thread id = " << std::this_thread::get_id() << '\n';
		mShutdown = true;

	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::LookupEventHandler(BaseModule::event_ptr)
///\ brief Looks up event handler register to event_ptr->mName
///\ param[in] event_item is a pointer to a BaseEvent
///\ returns pointer to callback (handler) registered with given event
///  or nullptr if no callback was registered with the event
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	BaseModule::cb_ptr BaseModule::LookupEventHandler(BaseModule::event_ptr event_item)
	{
		auto handler = mEventHandlerMap.find(event_item->GetName());
		if (handler == mEventHandlerMap.end())
		{
			return nullptr;
		}
		return handler->second;
	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::BaseModule()
///\ brief Default constructor
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	BaseModule::BaseModule() :
		Thread(),
		mId(++BaseModule::NextModuleId),
		mShutdown(false),
		mModuleState(MODULE_NOT_READY)
	{
	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::~BaseModule()
///\ brief Destructor
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	BaseModule::~BaseModule()
	{

	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::AddEvent(BaseModule::event_ptr)
///\ brief Adds event to modules mail back (called by other modules' SendToSubscribers
///  If handler is not found then event is silently dropped.
///\ param[in] event_item the event the module should handle (assuming appropriate handler is registered)
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void BaseModule::AddEvent(BaseModule::event_ptr event_item)
	{
		{
			/*	
				It is possible for a module to be launched, then have
				an event added (from another thread) before the module
				being launched has entered its event processing loop.
				This section will block the AddEvent() calling thread
				for up to some timeout period (currently 1 second!!).
				If the module has not become ready by the timeout period,
				then we exit without adding the event.

				TODO: Come up with way of notifying caller when an event was
				not added. Also, consider making the timeout period configurable to
				the module or add it as an optional parameter or both.
			 */
			std::unique_lock<std::mutex> locker(mUpdateStateMtx);
			while (mModuleState != MODULE_IS_READY && !mShutdown)
			{
				auto timeout = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000);
				if (mUpdateStateCond.wait_until(locker, timeout) == std::cv_status::timeout)
				{
					std::cout << INFO_MESSAGE("AddEvent() timed out waiting on module to become ready") << '\n';
					return;
				}
			}
			if (mShutdown)
			{
				std::cout << INFO_MESSAGE("AddEvent()    Module shutdown while trying to add event. Event was dropped.") << '\n';
				return;
			}
		}
		cb_ptr handler = LookupEventHandler(event_item);
		if (!handler)
		{
			// Need to handle this better
			//std::cout << "Thread: " << std::this_thread::get_id() << ' ' << __FUNCTION__ << " Module " << mId << " Handler for event " << event_item->GetName() << " not found. This event was dropped\n";
			return;
			//throw std::runtime_error(APP_ERROR_MESSAGE("Unhandled event")); // this is just for testing
		}
		mMailBox.AcceptMail({ event_item, handler });
	}

	
	
//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::AddSubscriber(std::shared_ptr<BaseModule>)
///\ brief Adds a module to the subscriber list of this module
///\ param[in] subscriber pointer to another module
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void BaseModule::AddSubscriber(std::shared_ptr<BaseModule> subscriber)
	{
		mSubscribers.push_back(subscriber);
	}


//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::SendToSubscribers(BaseModule::event_ptr)
///\ brief Adds event to mailbox of all modules subscribed to this module
///\ param[in] event_item event to for subscribers to handle
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void BaseModule::SendToSubscribers(BaseModule::event_ptr event_item)
	{
		for (auto subscriber : mSubscribers)
		{
			std::shared_ptr<BaseModule>(subscriber)->AddEvent(event_item);
		}
	}



//////////////////////////////////////////////////////////////////////
///\ fn BaseModule::GetId() const
///\ brief Returns modules' default assigned id.
///\ returns unique module id
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	const unsigned int& BaseModule::GetId() const
	{
		return mId;
	}


	BaseModule::event_ptr BaseModule::MakeEventPtr(const BaseModule::name_t& event_name)
	{
		return std::make_shared<BaseEvent>(event_name);
	}

#endif


template<class T>
void BaseModule::RegisterEventHandler(const BaseModule::name_t& event_name, void(T::*funct_ptr)(BaseModule::event_ptr))
{
	auto handler = mEventHandlerMap.find(event_name);
	if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
	}
	mEventHandlerMap[event_name] = std::make_shared<BaseModule::cb>(reinterpret_cast<void(BaseModule::*)(BaseModule::event_ptr)>(funct_ptr), this);
}

template<class T, class E>
void BaseModule::RegisterEventHandler(const BaseModule::name_t& event_name, void(T::*funct_ptr)(std::shared_ptr<E>))
{
	auto handler = mEventHandlerMap.find(event_name);
	if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
	}
	auto test_ptr = std::make_shared<BaseModule::cb>(reinterpret_cast<void(BaseModule::*)(BaseModule::event_ptr)>(funct_ptr), this);
	mEventHandlerMap[event_name] = std::make_shared<BaseModule::cb>(reinterpret_cast<void(BaseModule::*)(BaseModule::event_ptr)>(funct_ptr), this);
}

#endif