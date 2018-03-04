#ifndef EVENT_CC_INCLUDED
#define EVENT_CC_INCLUDED
#include"ebs/Event.h"

std::unordered_map<EventName::event_name_t, int> Event::GetRangeMap()
{
	std::unordered_map<EventName::event_name_t, int> result;
	for (unsigned int i = 0; gEventNames[i].mName != EventName::MAX_EVENT_NUMBER; ++i)
	{
		result[gEventNames[i].mName] = gEventNames[i + 1].mName - gEventNames[i].mName - 1;
	}
	return result;
}

std::shared_ptr<Event> MakeEventPtr(const EventName::event_name_t& event_name)
{
	return std::make_shared<Event>(event_name);
}
#endif