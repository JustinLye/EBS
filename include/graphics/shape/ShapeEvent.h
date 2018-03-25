#ifndef SHAPE_EVENT_HEADER_INCLUDED
#define SHAPE_EVENT_HEADER_INCLUDED
#include<glad/glad.h>
#include"ebs/BaseEvent.h"

class ShapeEvent :
	public BaseEvent
{
public:
	ShapeEvent(const name_t&, float* = nullptr);
	float* mDataPtr;
};

#endif