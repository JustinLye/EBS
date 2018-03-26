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
	void SubscribeToWindowEvents(std::shared_ptr<Module>);
	void SendOneOffEvent(std::shared_ptr<Event>);
};



#endif