#ifndef EVENT_NAME_HEADER_INCLUDED
#define EVENT_NAME_HEADER_INCLUDED

namespace EventName
{
	enum event_name_t : unsigned int
	{
		ERROREVENT, ERROREVENT_WHAT,
		SHUTDOWNEVENT,
		CLOSE_WINDOW,
		SHUTDOWN_WINDOW_EVENT,
		MOUSE_CLICK, MOUSE_CLICK_BUTTON, MOUSE_CLICK_ACTION, MOUSE_CLICK_MODS,
		CURSOR_POS, CURSOR_POS_X, CURSOR_POS_Y,
		TEXT_INPUT, TEXT_INPUT_CHAR,
		KEY_PRESS, KEY_PRESS_KEY, KEY_PRESS_SCANCODE, KEY_PRESS_ACTION, KEY_PRESS_MODS,
		MOUSE_SCROLL, MOUSE_SCROLL_X, MOUSE_SCROLL_Y,
		CURSOR_ENTER, CURSOR_ENTER_VAL,
		MAKE_CONTEXT_CURRENT,
		SHAPE_INITIALIZE,
		SHAPE_SHUTDOWN,
		MAX_EVENT_NUMBER
	};

	struct EventNameData
	{
		event_name_t mName;
		const char* mData[MAX_EVENT_NUMBER];
		bool operator<(const EventNameData& other) const
		{
			return mName < other.mName;
		}
		bool operator<=(const EventNameData& other) const
		{
			return mName <= other.mName;
		}
	};
}
#endif