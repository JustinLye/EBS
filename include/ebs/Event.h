#ifndef EVENT_HEADER_INCLUDED
#define EVENT_HEADER_INCLUDED

#include<memory>
#include<unordered_map>
#include<vector>
#include<boost/lexical_cast.hpp>
#include"util/UtilityDefs.h"
#include"ebs/EventName.cc"

//////////////////////////////////////////////
///\ class Event
///\ brief Generic container used to pass information from 
///  one event module to the next
//////////////////////////////////////////////

class Event
{
public:
	using name_t = EventName::event_name_t;
protected:
	name_t mName;
	std::vector<std::string> mFieldValues; ///< Holds data related to event. Data is stored as string, but can be cast to other types when retreived with Get()
	std::unordered_map<name_t, int> GetRangeMap(); ///< Maps each event "name" to range of event "data elements"
	int GetFieldRange(const name_t&); ///< Used to initialize mFieldValues.

public:
	Event(const name_t&); ///< Constructor that takes event name
	virtual ~Event(); ///< Destructor
	const name_t& GetName() const; ///< Return name of event
	void Set(const Event::name_t&, const std::string& field_value); ///< Set value of field using string
	std::string Get(const Event::name_t&) const; ///< Get value of field as string
	template<typename T>
	void Set(const Event::name_t&, const T&); ///< Set value of field using data of type T
	template<typename R>
	R Get(const Event::name_t&) const; ///< Get value of field as return type R
	
};

#ifndef EVENT_CC_INCLUDED
#include"ebs/Event.cc"
#endif

#endif