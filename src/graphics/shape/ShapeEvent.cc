#include"graphics/shape/ShapeEvent.h"

ShapeEvent::ShapeEvent(const ShapeEvent::name_t& event_name,float* data_ptr) :
	Event(event_name),
	mDataPtr(data_ptr)
{
}

