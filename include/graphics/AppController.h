#ifndef APP_CONTROLLER_HEADER_INCLUDED
#define APP_CONTROLLER_HEADER_INCLUDED

#include"graphics/window/WindowController.h"

class AppController
{
protected:
	std::shared_ptr<WindowController> mWindowController;
	GLFWwindow* mWindowPtr;

public:
	AppController();
	void Start();
	void Initialize();
	void EventLoop();
	void Shutdown();
	GLFWwindow* CreateSharedContext();
	GLFWwindow* GetWindowPtr();
	template<class EventType>
	void SubscribeToWindowEvents(std::shared_ptr<BaseModule<EventType>>);
	template<class EventType>
	void SendOneOffEvent(std::shared_ptr<BaseEvent<EventType>>);
};

template<class EventType>
void AppController::SubscribeToWindowEvents(std::shared_ptr<BaseModule<EventType>> module_ptr)
{
	WindowEventInterface::SubscribeModuleToWindowEvents(module_ptr);
}

template<class EventType>
void AppController::SendOneOffEvent(std::shared_ptr<BaseEvent<EventType>> event_ptr)
{
	WindowEventInterface::SendOneOffEvent(event_ptr);
}

#endif