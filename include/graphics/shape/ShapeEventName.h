#ifndef SHAPE_EVENT_NAME_HEADER_INCLUDED
#define SHAPE_EVENT_NAME_HEADER_INCLUDED

namespace EventName
{
	enum shape_event_name_t : unsigned int
	{
		SHAPE_INITIALIZE,
		SHAPE_SHUTDOWN,
		SHAPE_MAX_EVENT_NUM
	};
	struct ShapeEventData
	{
		shape_event_name_t mName;
		const char* mData[SHAPE_MAX_EVENT_NUM];
	};
};

#endif