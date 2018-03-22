#ifndef BASE_EVENT_CC_INCLUDED
#define BASE_EVENT_CC_INCLUDED
#include"ebs/BaseEvent.h"

#ifdef BUILD_BASE_EVENT_CC
std::unordered_map<BaseEvent::name_t, int> BaseEvent::GetRangeMap()
{
	std::unordered_map<name_t, int> result;
	for (unsigned int i = 0; gEventData[i].mName != name_t::MAX_EVENT_NUMBER; ++i)
	{
		result[gEventData[i].mName] = gEventData[i + 1].mName - gEventData[i].mName - 1;
	}
	return result;
}

int BaseEvent::GetFieldRange(const BaseEvent::name_t& event_name)
{
	std::unordered_map<name_t, int> range_map = GetRangeMap();
	auto range = range_map.find(event_name);
	if (range != range_map.end())
	{
		return range->second;
	} else
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown event name"));
	}
}
BaseEvent::BaseEvent(const BaseEvent::name_t& event_name) :
	mName(event_name)
{
		mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
}

BaseEvent::~BaseEvent()
{

}

const BaseEvent::name_t& BaseEvent::GetName() const
{
	return mName;
}

void BaseEvent::Set(const BaseEvent::name_t& field_name, const std::string& field_value)
{
	try
	{
		mFieldValues.at(field_name - mName - 1) = field_value;
	} catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	}
}

std::string BaseEvent::Get(const BaseEvent::name_t& field_name) const
{
	try
	{
		return mFieldValues.at(field_name - mName - 1);
	} catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	} catch (...)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown error occurred."));
	}
}

#endif

template<typename T>
void BaseEvent::Set(const BaseEvent::name_t& field_name, const T& field_value)
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
R BaseEvent::Get(const BaseEvent::name_t& field_name) const
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