#ifndef EVENT_MODULE_CC_INCLUDED
#define EVENT_MODULE_CC_INCLUDED
#include"ebs/EventModule.h"
#ifdef BUILD_EVENT_MODULE_CC
void EventModule::EntryPoint()
{
	if (!LookupEventHandler(MakeEventPtr(EventName::SHUTDOWNEVENT)))
	{
		RegisterEventHandler(EventName::SHUTDOWNEVENT, &EventModule::HandleShutdown);
	}
	EventLoop();
}

void EventModule::EventLoop()
{
	while (!mShutdown)
	{
		bound_event_handler handler = mMailBox.GetMail();
		(*handler.second)(handler.first);
	}
}

void EventModule::HandleShutdown(event_ptr event_item)
{
	std::cout << "Shutting down. Thread id = " << std::this_thread::get_id() << '\n';
	mShutdown = true;
}

event_handler_ptr EventModule::LookupEventHandler(event_ptr event_item)
{
	auto handler = mEventHandlerMap.find(event_item->GetName());
	if (handler == mEventHandlerMap.end())
	{
		return nullptr;
	}
	return handler->second;
}

EventModule::EventModule(const module_id_t& id) :
	mId(id),
	mShutdown(false),
	Thread()
{

}

EventModule::~EventModule()
{

}
void EventModule::AddEvent(event_ptr event_item)
{
	event_handler_ptr handler = LookupEventHandler(event_item);
	if (!handler)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unhandled event")); // this is just for testing
	}
	mMailBox.AcceptMail({ event_item, handler });
}

void EventModule::AddSubscriber(std::shared_ptr<EventModule> subscriber)
{
	mSubscribers.push_back(subscriber);
}

void EventModule::SendToSubscribers(event_ptr event_item)
{
	for (auto subscriber : mSubscribers)
	{
		std::shared_ptr<EventModule>(subscriber)->AddEvent(event_item);
	}
}
#endif


template<typename T>
void EventModule::RegisterEventHandler(name_t event_name, void(T::*funct_ptr)(event_ptr))
{
	auto handler = mEventHandlerMap.find(event_name);
	if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
	}
	mEventHandlerMap[event_name] = std::make_shared<CallBack<void, EventModule, event_ptr>>(funct_ptr, this);
}

#endif