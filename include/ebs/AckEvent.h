#ifndef ACK_EVENT_HEADER_INCLUDED
#define ACK_EVENT_HEADER_INCLUDED

#include"ebs/BaseEvent.h"



template<typename NameType>
class AckEvent :
	public BaseEvent<NameType>
{
protected:
	unsigned int mRequesterId; ///< Id used to identify event requester that requires an acknowledgement
public:
	AckEvent(const NameType&, unsigned int = 0);
	virtual ~AckEvent();
	const unsigned int& GetRequesterId() const; ///< Returns Requester Id
};

#ifndef ACK_EVENT_CC_INCLUDED
#include"ebs/AckEvent.cc"
#endif



#endif