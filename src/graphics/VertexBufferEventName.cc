#include"graphics/VertexBufferEventName.h"

namespace
{
	using namespace EventName;
	VertexBufferEventData gVertexBufferEventData[] =
	{
		{ VBUFF_REQUEST_VBO, {""}, 0, nullptr },
		{ VBUFF_REQUEST_VAO, {""}, 0, nullptr },
		{ VBUFF_ACK_VBO_REQUEST, {"VBOName"}, 0, nullptr },
		{ VBUFF_ACK_VAO_REQUEST, {"VAOName"}, 0, nullptr },
		{ VBUFF_SHUTDOWN, {""}, 0, nullptr },
		{ VBUFF_MAX_EVENT_NUM, {""}, 0, nullptr }
	};
};