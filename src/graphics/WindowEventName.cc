#include "graphics/WindowEventName.h"

namespace
{
	using namespace EventName;
	WindowEventData gWindowEventData[] =
	{
		{ CLOSE_WINDOW, {""}, nullptr },
		{ MOUSE_CLICK, {"Button","Action","Mods"}, nullptr },
		{ MAX_WINDOW_EVENT_NUM, {""}, nullptr }
	};
};