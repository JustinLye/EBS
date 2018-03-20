#ifndef ACK_EVENT_CC_INCLUDED
#define ACK_EVENT_CC_INCLUDED
#include"ebs/AckEvent.h"

template<class NameType>
AckEvent<NameType>::AckEvent(const NameType& event_name, unsigned int req_id) :
	BaseEvent<NameType>(event_name),
	mRequesterId(req_id)
{

}

template<class NameType>
AckEvent<NameType>::~AckEvent()
{

}

template<class NameType>
const unsigned int& AckEvent<NameType>::GetRequesterId() const
{
	return mRequesterId;
}
#endif