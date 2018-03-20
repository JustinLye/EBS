#ifndef WINDOW_EVENT_INTERFACE_HEADER
#define WINDOW_EVENT_INTERFACE_HEADER
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"graphics/window/WindowEventModule.h"

/*
	Window controller is designed to provide and interface between input events handled by GLFW and event modules
	that act on these input events. 

	One thing to note is any module needing notification of input events must be subscribed to the WindowController.
	WindowController will mainly act as a router by forwarding events to subscribers.

	4 things to do when added new callback
	1. Declare and define protected (i.e. internal) (e.g EHMouseClickCB) function void (WindowController::*)(std::shared_ptr<WindowEvent>)
	2. Declare and define public static function that matches sig. of GLFW callback you want to handle
	3. Register the internal cb with mInstance->mModule (see WindowController::Initialize)
	4. Bind static callback to glfw function in static WindowController::Bind

	Additionally, you will need to ensure there is a window_event_name_t setup for the event you're wanting to handle.
*/

class WindowEventInterface : protected WindowEventModule
{
private:
	
protected:
	static WindowEventInterface* mInstance;
	WindowEventModule* mModule;
	
	WindowEventInterface();
	
	void ForwardWindowEvent(std::shared_ptr<WindowEvent> event_ptr);
	bool IsModuleLaunched() const;
	static void Initialize(); // register callbacks etc.

public:
	static WindowEventInterface* GetInstance();
	static void Bind(GLFWwindow*);
	static void Start();
	static void Stop();
	static void SubscribeModuleToWindowEvents(std::shared_ptr<WindowEventModule>);
	static void MouseClickCB(GLFWwindow*, int, int, int);
	static void CursorPosCB(GLFWwindow*, double, double);
	static void CursorEnterCB(GLFWwindow*, int);
	static void MouseScrollCB(GLFWwindow*, double, double);
	static void CharInputCB(GLFWwindow*, unsigned int);
	static void KeyPressCB(GLFWwindow*, int, int, int, int);
	
};


#endif