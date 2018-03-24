#include"graphics/shape/ShapeEvent.h"

ShapeEvent::ShapeEvent(const ShapeEvent::name_t& event_name) :
	BaseEvent(event_name)
{
	mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
}

std::shared_ptr<ShapeEvent> MakeShapeEventPtr(const ShapeEvent::name_t& event_name)
{
	return std::make_shared<ShapeEvent>(event_name);
}