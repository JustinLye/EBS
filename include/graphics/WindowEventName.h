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
		CURSOR_POS, CURSOR_POS_X, CURSOR_POS_Y,
		TEXT_INPUT, TEXT_INPUT_CHAR,
		KEY_PRESS, KEY_PRESS_KEY, KEY_PRESS_SCANCODE, KEY_PRESS_ACTION, KEY_PRESS_MODS,
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