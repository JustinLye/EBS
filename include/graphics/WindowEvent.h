#ifndef  WINDOW_EVENT_HEADER_INCLUDED
#define WINDOW_EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"
#include"graphics/WindowEventName.cc"

class WindowEvent : public BaseEvent<EventName::window_event_name_t>
{
protected:
	std::unordered_map<EventName::window_event_name_t, int> GetRangeMap();
	GLFWwindow* mWindow;
public:
	explicit WindowEvent(const EventName::window_event_name_t&, GLFWwindow* = nullptr);
	GLFWwindow* GetWindowPtr();
};

typedef std::shared_ptr<WindowEvent> window_event_ptr;
window_event_ptr MakeWindowEventPtr(const EventName::window_event_name_t&, GLFWwindow* = nullptr);

#endif