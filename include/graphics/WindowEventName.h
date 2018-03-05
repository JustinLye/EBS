#ifndef WINDOWS_EVENT_NAME_HEADER_INCLUDED
#define WINDOWS_EVENT_NAME_HEADER_INCLUDED

#include"ebs/EventName.h"
struct GLFWwindow;
namespace EventName
{
	enum window_event_name_t : unsigned int
	{
		CLOSE_WINDOW,
		SHUTDOWN_WINDOW_EVENT,
		MOUSE_CLICK, MOUSE_CLICK_BUTTON, MOUSE_CLICK_ACTION, MOUSE_CLICK_MODS,
		MAX_WINDOW_EVENT_NUM
	};
	struct WindowEventData
	{
		window_event_name_t mName;
		const char* mData[MAX_WINDOW_EVENT_NUM];
		GLFWwindow* mWindow;
		
	};
};




#endif