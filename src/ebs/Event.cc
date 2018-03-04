#ifndef EVENT_CC_INCLUDED
#define EVENT_CC_INCLUDED
#include"ebs/Event.h"

#ifdef BUILD_EVENT_CC
Event::Event(const name_t& event_name) :
	mName(event_name),
	mFieldValues(GetFieldRange(event_name), "")
{
}

Event::range_map_t Event::GetRangeMap()
{
	range_map_t result;
	for (unsigned int i = 0; gEventNames[i].mName != EventName::MAX_EVENT_NUMBER; ++i)
	{
		result[gEventNames[i].mName] = gEventNames[i + 1].mName - gEventNames[i].mName - 1;
	}
	return result;
}
int Event::GetFieldRange(const name_t& event_name)
{
	range_map_t range_map = GetRangeMap();
	auto range = range_map.find(event_name);
	if (range != range_map.end())
	{
		return range->second;
	} else
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown event name"));
	}
}
void Event::Set(const name_t& field_name, const std::string& field_value)
{
	try
	{
		mFieldValues.at(field_name - mName - 1) = field_value;
	} catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	}
}
std::string Event::Get(const name_t& field_name) const
{
	try
	{
		return mFieldValues.at(field_name - mName - 1);
	}
	catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	} catch (...)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown error occurred."));
	}
}

std::shared_ptr<Event> MakeEventPtr(const EventName::event_name_t& event_name)
{
	return std::make_shared<Event>(event_name);
}

EventName::event_name_t Event::GetName() const
{
	return mName;
}

#endif

template<typename T>
void Event::Set(const name_t& field_name, const T& field_value)
{
	try
	{
		Set(field_name, boost::lexical_cast<std::string>(field_value));
	} catch (boost::bad_lexical_cast& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	}
}

template<typename R>
R Event::Get(const name_t& field_name) const
{
	try
	{
		std::string value = mFieldValues.at(field_name - mName - 1);
		return boost::lexical_cast<R>(value);
	} catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	} catch (...)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown error occurred."));
	}
}
#endif