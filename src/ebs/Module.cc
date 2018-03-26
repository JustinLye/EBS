#ifndef MODULE_CC_INCLUDED
#define MODULE_CC_INCLUDED
#include"ebs/Module.h"
#ifdef BUILD_MODULE_CC

unsigned int Module::NextModuleId = 0;



//////////////////////////////////////////////////////////////////////
///\ fn EntryPoint()
///\ brief Entry point when thread is launched
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void Module::EntryPoint()
	{
		if (!LookupEventHandler(MakeEventPtr(name_t::SHUTDOWNEVENT)))
		{
			RegisterEventHandler(name_t::SHUTDOWNEVENT, &Module::HandleShutdown);
		}
		EventLoop();
	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::GetMail()
///\ brief Entry point when thread is launched
///\ returns Next message in mailbox queue
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	Module::bound_cb Module::GetMail()
	{
		return mMailBox.GetMail();
	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::EventLoop()
///\ brief Loop that runs until mShutdown is set to true
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


		void Module::EventLoop()
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
///\ fn Module::HandleShutdown(Module::event_ptr)
///\ brief Loop that runs until mShutdown is set to true
///\ param[in] event_item is a pointer to a Event w/ name SHUTDOWN
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void Module::HandleShutdown(Module::event_ptr event_item)
	{
		{
			std::lock_guard<std::mutex> locker(mUpdateStateMtx);
			mModuleState = MODULE_NOT_READY;
			mShutdown = true;
		}
		std::cout << "Shutting down. Thread id = " << std::this_thread::get_id() << '\n';
		

	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::LookupEventHandler(Module::event_ptr)
///\ brief Looks up event handler register to event_ptr->mName
///\ param[in] event_item is a pointer to a Event
///\ returns pointer to callback (handler) registered with given event
///  or nullptr if no callback was registered with the event
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	Module::cb_ptr Module::LookupEventHandler(Module::event_ptr event_item)
	{
		auto handler = mEventHandlerMap.find(event_item->GetName());
		if (handler == mEventHandlerMap.end())
		{
			return nullptr;
		}
		return handler->second;
	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::Module()
///\ brief Default constructor
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	Module::Module() :
		Thread(),
		mId(++Module::NextModuleId),
		mShutdown(false),
		mModuleState(MODULE_NOT_READY)
	{
	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::~Module()
///\ brief Destructor
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	Module::~Module()
	{

	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::AddEvent(Module::event_ptr)
///\ brief Adds event to modules mail back (called by other modules' SendToSubscribers
///  If handler is not found then event is silently dropped.
///\ param[in] event_item the event the module should handle (assuming appropriate handler is registered)
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void Module::AddEvent(Module::event_ptr event_item)
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
///\ fn Module::AddSubscriber(std::shared_ptr<Module>)
///\ brief Adds a module to the subscriber list of this module
///\ param[in] subscriber pointer to another module
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void Module::AddSubscriber(std::shared_ptr<Module> subscriber)
	{
		mSubscribers.push_back(subscriber);
	}


//////////////////////////////////////////////////////////////////////
///\ fn Module::SendToSubscribers(Module::event_ptr)
///\ brief Adds event to mailbox of all modules subscribed to this module
///\ param[in] event_item event to for subscribers to handle
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	void Module::SendToSubscribers(Module::event_ptr event_item)
	{
		for (auto subscriber : mSubscribers)
		{
			std::shared_ptr<Module>(subscriber)->AddEvent(event_item);
		}
	}



//////////////////////////////////////////////////////////////////////
///\ fn Module::GetId() const
///\ brief Returns modules' default assigned id.
///\ returns unique module id
///\ author Justin Lye
///\ date 03/21/2018
//////////////////////////////////////////////////////////////////////


	const unsigned int& Module::GetId() const
	{
		return mId;
	}


	Module::event_ptr Module::MakeEventPtr(const Module::name_t& event_name)
	{
		return std::make_shared<Event>(event_name);
	}

	bool Module::IsShuttingDown() const
	{
		return mShutdown;
	}

#endif


template<class T>
void Module::RegisterEventHandler(const Module::name_t& event_name, void(T::*funct_ptr)(Module::event_ptr))
{
	auto handler = mEventHandlerMap.find(event_name);
	if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
	}
	mEventHandlerMap[event_name] = std::make_shared<Module::cb>(reinterpret_cast<void(Module::*)(Module::event_ptr)>(funct_ptr), this);
}

template<class T, class E>
void Module::RegisterEventHandler(const Module::name_t& event_name, void(T::*funct_ptr)(std::shared_ptr<E>))
{
	auto handler = mEventHandlerMap.find(event_name);
	if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
	}
	auto test_ptr = std::make_shared<Module::cb>(reinterpret_cast<void(Module::*)(Module::event_ptr)>(funct_ptr), this);
	mEventHandlerMap[event_name] = std::make_shared<Module::cb>(reinterpret_cast<void(Module::*)(Module::event_ptr)>(funct_ptr), this);
}

#endif