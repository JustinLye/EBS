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
		{ SHAPE_INITIALIZE,{ "" } },
		{ SHAPE_SHUTDOWN,{ "" } },
		{ SHAPE_ADDSHAPE, {"VAO", "VBO", "Length"}},
		{ SHAPE_ADDSHAPE_COMPLETE, {""}},
		{ RENDER_DRAW, {""}},
		{ RENDER_DRAWING_COMPLETE, {"ModuleId"}},
		{ RENDER_CONTEXT_CHANGE_RELEASE, {""}},
		{ RENDER_CONTEXT_CHANGE_CONTROL, {""}},
		{ RENDER_CONTEXT_CHANGE_COMPLETE, {"RenderContextChangeWasReleaseType"}},
		{ MAX_EVENT_NUMBER, {""}}
	};
}