#ifndef SHAPE_EVENT_HEADER_INCLUDED
#define SHAPE_EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"

class ShapeEvent :
	public BaseEvent
{
	
public:
	ShapeEvent(const name_t&);
};

typedef std::shared_ptr<ShapeEvent> shape_event_ptr;
shape_event_ptr MakeShapeEventPtr(const ShapeEvent::name_t&);

#endif