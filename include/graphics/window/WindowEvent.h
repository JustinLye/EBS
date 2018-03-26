#ifndef  WINDOW_EVENT_HEADER_INCLUDED
#define WINDOW_EVENT_HEADER_INCLUDED

#include"ebs/Event.h"
struct GLFWwindow;
class WindowEvent :
	public Event
{
public:
	using name_t = EventName::event_name_t;
protected:
	GLFWwindow* mWindow;
public:
	WindowEvent(const name_t&, GLFWwindow* = nullptr);
	GLFWwindow* GetWindowPtr();
	static std::shared_ptr<WindowEvent> MakeWindowEventPtr(const WindowEvent::name_t&, GLFWwindow* = nullptr);
};


#endif