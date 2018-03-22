#include"ebs/EventName.h"

namespace {
using namespace EventName;
	EventNameData gEventData[] =
	{
		{ ERROREVENT, {"ErrorEvent", "<What>%s</What>\n"}},
		{ SHUTDOWNEVENT, {""}},
		{ DEFAULT_EVENT, {""}},
		{ MAX_EVENT_NUMBER, {""}}
	};
}