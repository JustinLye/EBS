#ifndef VERTEX_BUFFER_EVENT_MODULE_HEADER_INCLUDED
#define VERTEX_BUFFER_EVENT_MODULE_HEADER_INCLUDED

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"ebs/BaseModule.h"
#include"graphics/VertexBufferEvent.h"


class VertexBufferEventModule :
	public BaseModule<VertexBufferEvent>
{
protected:
	virtual void GenerateVBO(event_ptr);
	virtual void GenerateVAO(event_ptr);
public:
	VertexBufferEventModule();
	virtual ~VertexBufferEventModule();
	virtual void SendToSubscribers(event_ptr) override;
};

#endif