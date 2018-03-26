#include"graphics/window/WindowController.h"


WindowController::WindowController() :
	Module(),
	mModule(std::make_shared<Module>())
{
	mEventInterface = WindowEventInterface::GetInstance();
	WindowEventInterface::SubscribeModuleToWindowEvents(mModule);
	mModule->RegisterEventHandler(EventName::KEY_PRESS, &WindowController::HandleKeyPress);
	mModule->Launch();
}

WindowController::~WindowController()
{
	if (mModule->joinable())
	{
		auto event_ptr = std::make_shared<Event>(EventName::SHUTDOWNEVENT);
		mModule->AddEvent(event_ptr);
		mModule->join();
	}
}

void WindowController::HandleKeyPress(std::shared_ptr<WindowEvent> event_ptr)
{
	int key = event_ptr->Get<int>(EventName::KEY_PRESS_KEY);
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		break;
	case GLFW_KEY_SPACE:
		break;
	}
	if (event_ptr->Get<int>(EventName::KEY_PRESS_KEY) == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(event_ptr->GetWindowPtr(), GLFW_TRUE);
	}
	
}
