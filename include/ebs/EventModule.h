#ifndef EVENT_HANDLER_HEADER_INCLUDED
#define EVENT_HANDLER_HEADER_INCLUDED
#include"ebs/Event.h"
#include"ebs/BaseModule.h"


class EventModule : public BaseModule<Event>
{
	void Test(std::shared_ptr<Event>);
public:
	explicit EventModule();
	virtual ~EventModule();

};

EventModule::EventModule() :
	BaseModule<Event>()
{
	RegisterEventHandler(EventName::ERROREVENT, &EventModule::Test);
}

EventModule::~EventModule()
{

}

void EventModule::Test(std::shared_ptr<Event> event_ptr)
{
	std::cout << event_ptr->Get<std::string>(EventName::ERROREVENT_WHAT) << '\n';
}

#endif