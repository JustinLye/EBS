#include"graphics/VertexBufferEvent.h"

VertexBufferEvent::VertexBufferEvent(const EventName::vbuff_event_name_t& event_name, unsigned int req_id, GLuint* vao_ptr) :
	AckEvent<EventName::vbuff_event_name_t>(event_name, req_id),
	mVAO(vao_ptr)
{
	mFieldValues = std::vector<std::string>(GetFieldRange(event_name), "");
}

std::unordered_map<EventName::vbuff_event_name_t, int> VertexBufferEvent::GetRangeMap()
{
	std::unordered_map<EventName::vbuff_event_name_t, int> result;
	for (unsigned int i = 0; gVertexBufferEventData[i].mName != EventName::VBUFF_MAX_EVENT_NUM; ++i)
	{
		result[gVertexBufferEventData[i].mName] = gVertexBufferEventData[i + 1].mName - gVertexBufferEventData[i].mName - 1;
	}
	return result;
}

std::shared_ptr<VertexBufferEvent> MakeVBuffEventPtr(const EventName::vbuff_event_name_t& event_name, unsigned int req_id, GLuint* mVAO)
{
	return std::make_shared<VertexBufferEvent>(event_name, req_id, mVAO);
}