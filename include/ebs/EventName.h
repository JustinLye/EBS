#ifndef EVENT_NAME_HEADER_INCLUDED
#define EVENT_NAME_HEADER_INCLUDED

namespace EventName
{
	enum event_name_t : unsigned int
	{
		ERROREVENT, ERROREVENT_WHAT,
		SHUTDOWNEVENT,
		DEFAULT_EVENT,
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