#ifndef VERTEX_BUFFER_EVENT_NAME_HEADER_INCLUDED
#define VERTEX_BUFFER_EVENT_NAME_HEADER_INCLUDED
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"ebs/EventName.h"

namespace EventName
{
	enum vbuff_event_name_t : unsigned int
	{
		VBUFF_REQUEST_VBO, 
		VBUFF_REQUEST_VAO,
		VBUFF_ACK_VBO_REQUEST, VBUFF_VBO_NAME,
		VBUFF_ACK_VAO_REQUEST, VBUFF_VAO_NAME,
		VBUFF_SHUTDOWN,
		VBUFF_MAX_EVENT_NUM
	};
	struct VertexBufferEventData
	{
		vbuff_event_name_t mName;
		const char* mData[VBUFF_MAX_EVENT_NUM];
		unsigned int mRequesterId;
		GLuint* mVAO;
		
	};
};

#endif