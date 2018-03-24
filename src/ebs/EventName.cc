#include"ebs/EventName.h"

namespace {
using namespace EventName;
	EventNameData gEventData[] =
	{
		{ ERROREVENT, {"ErrorEvent", "<What>%s</What>\n"}},
		{ SHUTDOWNEVENT, {""}},
		{ CLOSE_WINDOW,{ "" }},
		{ SHUTDOWN_WINDOW_EVENT,{ "" }},
		{ MOUSE_CLICK,{ "Button","Action","Mods" }},
		{ CURSOR_POS,{ "x","y" }},
		{ TEXT_INPUT,{ "Text" }},
		{ KEY_PRESS,{ "Key","Scancode","Action","Mods" }},
		{ MOUSE_SCROLL,{ "Xoffset", "Yoffset" }},
		{ CURSOR_ENTER,{ "CursorEntered" }},
		{ MAKE_CONTEXT_CURRENT,{ "" }},
		{ SHAPE_INITIALIZE,{ "" } },
		{ SHAPE_SHUTDOWN,{ "" } },
		{ MAX_EVENT_NUMBER, {""}}
	};
}