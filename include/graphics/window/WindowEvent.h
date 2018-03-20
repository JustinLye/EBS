#ifndef  WINDOW_EVENT_HEADER_INCLUDED
#define WINDOW_EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"
#include"graphics/window/WindowEventName.cc"

class WindowEvent : public BaseEvent<EventName::window_event_name_t>
{
protected:
	std::unordered_map<EventName::window_event_name_t, int> GetRangeMap();
	GLFWwindow* mWindow;
public:
	using name_type = typename EventName::window_event_name_t;
	static const name_type shutdown_event = EventName::SHUTDOWN_WINDOW_EVENT;
	WindowEvent(const EventName::window_event_name_t&, GLFWwindow* = nullptr);
	//explicit WindowEvent(const EventName::window_event_name_t&, GLFWwindow* = nullptr);
	GLFWwindow* GetWindowPtr();
};

typedef std::shared_ptr<WindowEvent> window_event_ptr;
window_event_ptr MakeWindowEventPtr(const EventName::window_event_name_t&, GLFWwindow* = nullptr);

#endif