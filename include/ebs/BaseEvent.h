#ifndef BASE_EVENT_HEADER_INCLUDED
#define BASE_EVENT_HEADER_INCLUDED

#include<memory>
#include<unordered_map>
#include<vector>
#include<boost/lexical_cast.hpp>
#include"util/UtilityDefs.h"

//////////////////////////////////////////////
///\ class CallBack
///\ brief Provides container for member
///  function calls that take a single argument
//////////////////////////////////////////////

template<typename NameType>
class BaseEvent
{
protected:
	NameType mName;
	std::vector<std::string> mFieldValues;
	virtual std::unordered_map<NameType, int> GetRangeMap() = 0;
	int GetFieldRange(const NameType& event_name)
	{
		std::unordered_map<NameType, int> range_map = GetRangeMap();
		auto range = range_map.find(event_name);
		if (range != range_map.end())
		{
			return range->second;
		} else
		{
			throw std::runtime_error(APP_ERROR_MESSAGE("Unknown event name"));
		}
	}
public:
	virtual ~BaseEvent() {}
	void Set(const NameType& field_name, const std::string& field_value)
	{
		try
		{
			mFieldValues.at(field_name - mName - 1) = field_value;
		} catch (std::exception& error)
		{
			throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
		}
	}
	template<typename T>
	void Set(const NameType& field_name, const T& field_value)
	{
		try
		{
			Set(field_name, boost::lexical_cast<std::string>(field_value));
		} catch (boost::bad_lexical_cast& error)
		{
			throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
		}
	}
	std::string Get(const NameType& field_name) const
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
	template<typename R>
	R Get(const NameType& field_name) const
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
	NameType GetName() const
	{
		return mName;
	}
};

#ifndef BASE_EVENT_CC_INCLUDED
#include"ebs/BaseEvent.cc"
#endif

#endif