#ifndef EVENT_HEADER_INCLUDED
#define EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"
#include"ebs/EventName.cc"

//extern EventName::EventNameData gEventNames[];

//////////////////////////////////////////////
///\ class CallBack
///\ brief Provides container for member
///  function calls that take a single argument
//////////////////////////////////////////////

class Event : public BaseEvent<EventName::event_name_t>
{
protected:
	typedef EventName::event_name_t name_t;
	std::unordered_map<EventName::event_name_t, int> GetRangeMap();
public:
	using name_type = typename EventName::event_name_t;
	static const name_type shutdown_event = EventName::SHUTDOWNEVENT;
	explicit Event(const EventName::event_name_t event_name) :
		BaseEvent<EventName::event_name_t>(event_name)
	{
		//mName = event_name;
		mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
	}
};

typedef std::shared_ptr<Event> event_ptr;
event_ptr MakeEventPtr(const EventName::event_name_t& event_name);

#endif
