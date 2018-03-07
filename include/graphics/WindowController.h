#ifndef WINDOW_CONTROLLER_HEADER_INCLUDED
#define WINDOW_CONTROLLER_HEADER_INCLUDED

#include"graphics/WindowEventInterface.h"

class WindowController :
	private WindowEventModule
{
protected:
	std::shared_ptr<WindowEventModule> mModule;
	WindowEventInterface* mEventInterface;
	void HandleKeyPress(std::shared_ptr<WindowEvent>);
public:
	WindowController();
	virtual ~WindowController();
};

WindowController::WindowController() :
	WindowEventModule(1),
	mModule(std::make_shared<WindowEventModule>(1))
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
		auto event_ptr = std::make_shared<WindowEvent>(EventName::SHUTDOWN_WINDOW_EVENT, nullptr);
		mModule->AddEvent(event_ptr);
		mModule->join();
	}
}

void WindowController::HandleKeyPress(std::shared_ptr<WindowEvent> event_ptr)
{
	if (event_ptr->Get<int>(EventName::KEY_PRESS_KEY) == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(event_ptr->GetWindowPtr(), GLFW_TRUE);
	}
}


#endif