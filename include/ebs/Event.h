#ifndef EVENT_HEADER_INCLUDED
#define EVENT_HEADER_INCLUDED

#include<memory>
#include<unordered_map>
#include<vector>
#include<boost/lexical_cast.hpp>
#include"util/UtilityDefs"
#include"ebs/EventName.cc"

//extern EventName::EventNameData gEventNames[];

//////////////////////////////////////////////
///\ class CallBack
///\ brief Provides container for member
///  function calls that take a single argument
//////////////////////////////////////////////

class Event
{
	typedef EventName::event_name_t name_t;
	typedef std::unordered_map<name_t, const char **> info_map_t;
	typedef std::unordered_map<name_t, int> range_map_t;
	name_t mName;
	std::vector<std::string> mFieldValues;
	static range_map_t GetRangeMap();
	static int GetFieldRange(const name_t&);
public:
	explicit Event(const name_t&);
	void Set(const name_t&, const std::string&);
	template<typename T>
	void Set(const name_t&, const T&);
	std::string Get(const name_t&) const;
	template<typename R>
	R Get(const name_t&) const;
	name_t GetName() const;
};

typedef std::shared_ptr<Event> event_ptr;
event_ptr MakeEventPtr(const EventName::event_name_t& event_name);

#ifndef EVENT_CC_INCLUDED
#include"ebs/Event.cc"
#endif
#endif
