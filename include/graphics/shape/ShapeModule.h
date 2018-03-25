#ifndef SHAPE_MODULE_HEADER_INCLUDED
#define SHAPE_MODULE_HEADER_INCLUDED

#include<forward_list>
#include"ebs/BaseModule.h"
#include"graphics/shape/BaseShape.h"
#include"graphics/shape/ShapeEvent.h"
#include"graphics/window/WindowEvent.h"
class ShapeModule :
	public BaseModule
{
public:
	using vao_t = GLuint;
	using vbo_t = GLuint;
protected:
	std::unordered_map<vao_t, std::forward_list<std::shared_ptr<BaseShape>>> mShapeMap;
public:
	ShapeModule();
	virtual ~ShapeModule();
	virtual void HandleContextEvent(std::shared_ptr<WindowEvent>);
	virtual void HandleDrawEvent(std::shared_ptr<BaseEvent>);
	virtual void HandleAddShapeEvent(std::shared_ptr<ShapeEvent>);

};

ShapeModule::ShapeModule() :
	BaseModule()
{
	RegisterEventHandler(EventName::RENDER_CONTEXT_CHANGE_RELEASE, &ShapeModule::HandleContextEvent);
	RegisterEventHandler(EventName::RENDER_CONTEXT_CHANGE_CONTROL, &ShapeModule::HandleContextEvent);
	RegisterEventHandler(EventName::RENDER_DRAW, &ShapeModule::HandleDrawEvent);
	RegisterEventHandler(EventName::SHAPE_ADDSHAPE, &ShapeModule::HandleAddShapeEvent);
}

ShapeModule::~ShapeModule()
{

}

void ShapeModule::HandleContextEvent(std::shared_ptr<WindowEvent> event_ptr)
{
	if (event_ptr->GetName() == EventName::RENDER_CONTEXT_CHANGE_RELEASE)
	{
		if (glfwGetCurrentContext() == event_ptr->GetWindowPtr())
		{
			glfwMakeContextCurrent(NULL);
		}
		std::shared_ptr<BaseEvent> ack_event = std::make_shared<BaseEvent>(EventName::RENDER_CONTEXT_CHANGE_COMPLETE);
		ack_event->Set<int>(EventName::RENDER_CONTEXT_CHANGE_WAS_RELEASE_TYPE, 1);
		SendToSubscribers(ack_event);
	}
	else
	{
		if (glfwGetCurrentContext() != event_ptr->GetWindowPtr())
		{
			glfwMakeContextCurrent(event_ptr->GetWindowPtr());
		}
		std::shared_ptr<BaseEvent> ack_event = std::make_shared<BaseEvent>(EventName::RENDER_CONTEXT_CHANGE_COMPLETE);
		ack_event->Set<int>(EventName::RENDER_CONTEXT_CHANGE_WAS_RELEASE_TYPE, 0);
		SendToSubscribers(ack_event);
	}
}

void ShapeModule::HandleDrawEvent(std::shared_ptr<BaseEvent> event_ptr)
{
	for (auto vao : mShapeMap)
	{
		glBindVertexArray(vao.first);
		for (auto shape : vao.second)
		{
			shape->Draw();
		}
		glBindVertexArray(0);
	}
	std::shared_ptr<BaseEvent> ack_event = std::make_shared<BaseEvent>(EventName::RENDER_DRAWING_COMPLETE);
	ack_event->Set<unsigned int>(EventName::RENDER_DRAWING_COMPLETE_MODULE_ID, mId);
	SendToSubscribers(ack_event);
}

void ShapeModule::HandleAddShapeEvent(std::shared_ptr<ShapeEvent> event_ptr)
{
	std::shared_ptr<BaseShape> shape = std::make_shared<BaseShape>();
	GLuint vao = event_ptr->Get<GLuint>(EventName::SHAPE_ADDSHAPE_VAO);
	GLuint vbo = event_ptr->Get<GLuint>(EventName::SHAPE_ADDSHAPE_VBO);

	shape->Initialize(vbo);
	shape->SetPointData(event_ptr->mDataPtr, event_ptr->Get<unsigned int>(EventName::SHAPE_ADDSHAPE_DATA_LENGTH));
	if (mShapeMap.find(vao) == mShapeMap.end())
	{
		mShapeMap.insert({ vao,std::forward_list<std::shared_ptr<BaseShape>>() });
	}
	mShapeMap[vao].push_front(shape);
	std::shared_ptr<BaseEvent> ack_event = std::make_shared<BaseEvent>(EventName::SHAPE_ADDSHAPE_COMPLETE);
	SendToSubscribers(ack_event);
}

#endif
