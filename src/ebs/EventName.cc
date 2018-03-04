#include"ebs/EventName.h"

namespace {
using namespace EventName;
	EventNameData gEventNames[] =
	{
		{ ERROREVENT, {"ErrorEvent", "<What>%s</What>\n"}},
		{ SHUTDOWNEVENT, {""}},
		{ MAX_EVENT_NUMBER, {""}}
	};
}