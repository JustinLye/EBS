#ifndef VERTEX_BUFFER_EVENT_HEADER_INCLUDED
#define VERTEX_BUFFER_EVENT_HEADER_INCLUDED

#include"ebs/AckEvent.h"
#include"graphics/VertexBufferEventName.cc"

class VertexBufferEvent :
	public AckEvent<EventName::vbuff_event_name_t>
{
protected:
	std::unordered_map<EventName::vbuff_event_name_t, int> GetRangeMap();
public:
	using name_type = typename EventName::vbuff_event_name_t;
	static const name_type shutdown_event = EventName::VBUFF_SHUTDOWN;
	GLuint* mVAO;
	VertexBufferEvent(const EventName::vbuff_event_name_t&, unsigned int = 0, GLuint* = 0);

};

typedef std::shared_ptr<VertexBufferEvent> vbuff_event_ptr;
vbuff_event_ptr MakeVBuffEventPtr(const EventName::vbuff_event_name_t&, unsigned int = 0, GLuint* = 0);

#endif
