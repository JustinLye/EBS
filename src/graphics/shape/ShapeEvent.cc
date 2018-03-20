#include"graphics/shape/ShapeEvent.h"

std::unordered_map<EventName::shape_event_name_t, int> ShapeEvent::GetRangeMap()
{
	std::unordered_map<EventName::shape_event_name_t, int> result;
	for (unsigned int i = 0; gShapeEventData[i].mName != EventName::SHAPE_MAX_EVENT_NUM; ++i)
	{
		result[gShapeEventData[i].mName] = gShapeEventData[i + 1].mName - gShapeEventData[i].mName - 1;
	}
	return result;
}

ShapeEvent::ShapeEvent(const EventName::shape_event_name_t& event_name) :
	BaseEvent<EventName::shape_event_name_t>(event_name)
{
	mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
}

std::shared_ptr<ShapeEvent> MakeShapeEventPtr(const EventName::shape_event_name_t& event_name)
{
	return std::make_shared<ShapeEvent>(event_name);
}