#include"graphics/VertexBufferEventModule.h"


void VertexBufferEventModule::GenerateVBO(VertexBufferEventModule::event_ptr event_item)
{
	unsigned int result_vbo;
	glGenBuffers(1, &result_vbo);
	auto event_p = MakeVBuffEventPtr(EventName::VBUFF_ACK_VBO_REQUEST, event_item->GetRequesterId());
	event_p->Set<unsigned int>(EventName::VBUFF_VBO_NAME, result_vbo);
	SendToSubscribers(event_p);
}

void VertexBufferEventModule::GenerateVAO(VertexBufferEventModule::event_ptr event_item)
{
	std::cout << __FUNCTION__ << ' ' << __LINE__ << ' ' << *event_item->mVAO << '\n';
	glGenVertexArrays(1, event_item->mVAO);
	std::cout << __FUNCTION__ << ' ' << __LINE__ << ' ' << *event_item->mVAO << '\n';
	auto event_p = MakeVBuffEventPtr(EventName::VBUFF_ACK_VAO_REQUEST, event_item->GetRequesterId(), event_item->mVAO);
	SendToSubscribers(event_p);
}

VertexBufferEventModule::VertexBufferEventModule() :
	BaseModule<VertexBufferEvent>()
{
	RegisterEventHandler(EventName::VBUFF_REQUEST_VAO, &VertexBufferEventModule::GenerateVAO);
	RegisterEventHandler(EventName::VBUFF_REQUEST_VBO, &VertexBufferEventModule::GenerateVBO);
}

VertexBufferEventModule::~VertexBufferEventModule()
{
}

void VertexBufferEventModule::SendToSubscribers(VertexBufferEventModule::event_ptr event_item)
{
	// TODO: add drop event logging
	for (auto subscriber : mSubscribers)
	{
		if (event_item->GetRequesterId() == std::shared_ptr<BaseModule<VertexBufferEvent>>(subscriber)->GetId())
		{
			std::shared_ptr<BaseModule<VertexBufferEvent>>(subscriber)->AddEvent(event_item);
		}
	}
}