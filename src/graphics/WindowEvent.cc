#include"graphics/WindowEvent.h"

WindowEvent::WindowEvent(const EventName::window_event_name_t& event_name, GLFWwindow* window) :
	BaseEvent<EventName::window_event_name_t>(event_name),
	mWindow(window)
{
	mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
}

/*WindowEvent::WindowEvent(const EventName::window_event_name_t& event_name, GLFWwindow* window) :
	BaseEvent<EventName::window_event_name_t>()
{
	mName = event_name;
	mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
	mWindow = window;
}*/

std::unordered_map<EventName::window_event_name_t, int> WindowEvent::GetRangeMap()
{
	std::unordered_map<EventName::window_event_name_t, int> result;
	for (unsigned int i = 0; gWindowEventData[i].mName != EventName::MAX_WINDOW_EVENT_NUM; ++i)
	{
		result[gWindowEventData[i].mName] = gWindowEventData[i + 1].mName - gWindowEventData[i].mName - 1;
	}
	return result;
}

GLFWwindow* WindowEvent::GetWindowPtr()
{
	return mWindow;
}

std::shared_ptr<WindowEvent> MakeWindowEventPtr(const EventName::window_event_name_t& event_name, GLFWwindow* window)
{
	return std::make_shared<WindowEvent>(event_name, window);
}