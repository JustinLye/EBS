#ifndef SHAPE_EVENT_HEADER_INCLUDED
#define SHAPE_EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"
#include"graphics/shape/ShapeEventName.cc"

class ShapeEvent :
	public BaseEvent<EventName::shape_event_name_t>
{
protected:
	std::unordered_map<EventName::shape_event_name_t, int> GetRangeMap();
public:
	using name_type = typename EventName::shape_event_name_t;
	static const name_type shutdown_event = EventName::SHAPE_SHUTDOWN;
	ShapeEvent(const EventName::shape_event_name_t&);
};

typedef std::shared_ptr<ShapeEvent> shape_event_ptr;
shape_event_ptr MakeShapeEventPtr(const EventName::shape_event_name_t&);

#endif