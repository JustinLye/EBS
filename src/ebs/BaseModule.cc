#ifndef BASE_MODULE_CC_INCLUDED
#define BASE_MODULE_CC_INCLUDED
#include"ebs/BaseModule.h"
//#ifdef BUILD_BASE_EVENT_MODULE_CC

template<class EventType>
unsigned int BaseModule<EventType>::NextModuleId = 0;

template<class EventType>
void BaseModule<EventType>::EntryPoint()
{
	static_assert(EventType::shutdown_event, "EventType must have shutdown_event and shutdown_event cannot map to zero.");
	if (!LookupEventHandler(MakeEventPtr(EventType::shutdown_event)))
	{
		RegisterEventHandler(EventType::shutdown_event, &BaseModule::HandleShutdown);
	}
	EventLoop();
}

template<class EventType>
std::pair <std::shared_ptr<EventType>, std::shared_ptr<CallBack<void, BaseModule<EventType>, std::shared_ptr<EventType>>>> GetMail()
{
	return mMailBox.GetMail();
}

template<class EventType>
void BaseModule<EventType>::EventLoop()
{
	while (!mShutdown)
	{
		bound_cb handler = mMailBox.GetMail();
		(*handler.second)(handler.first);
	}
}

template<class EventType>
void BaseModule<EventType>::HandleShutdown(std::shared_ptr<EventType> event_item)
{
	std::cout << "Shutting down. Thread id = " << std::this_thread::get_id() << '\n';
	mShutdown = true;
}

template<class EventType>
std::shared_ptr<CallBack<void, BaseModule<EventType>, std::shared_ptr<EventType>>> BaseModule<EventType>::LookupEventHandler(std::shared_ptr<EventType> event_item)
{
	auto handler = mEventHandlerMap.find(event_item->GetName());
	if (handler == mEventHandlerMap.end())
	{
		return nullptr;
	}
	return handler->second;
}

template<class EventType>
BaseModule<EventType>::BaseModule() :
	mId(++BaseModule<EventType>::NextModuleId),
	mShutdown(false),
	Thread()
{

}
template<class EventType>
BaseModule<EventType>::~BaseModule()
{

}
template<class EventType>
void BaseModule<EventType>::AddEvent(std::shared_ptr<EventType> event_item)
{
	cb_ptr handler = LookupEventHandler(event_item);
	if (!handler)
	{
		// Need to handle this better
		return;
		//throw std::runtime_error(APP_ERROR_MESSAGE("Unhandled event")); // this is just for testing
	}
	mMailBox.AcceptMail({ event_item, handler });
}

template<class EventType>
void BaseModule<EventType>::AddSubscriber(std::shared_ptr<BaseModule> subscriber)
{
	mSubscribers.push_back(subscriber);
}

template<class EventType>
void BaseModule<EventType>::SendToSubscribers(std::shared_ptr<EventType> event_item)
{
	for (auto subscriber : mSubscribers)
	{
		std::shared_ptr<BaseModule>(subscriber)->AddEvent(event_item);
	}
}

template<class EventType>
const unsigned int& BaseModule<EventType>::GetId() const
{
	return mId;
}
//#endif

#endif