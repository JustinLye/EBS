#ifndef WINDOW_EVENT_INTERFACE_HEADER
#define WINDOW_EVENT_INTERFACE_HEADER
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"graphics/WindowEventModule.h"

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
	//friend class WindowController;

	bool IsModuleLaunched() const;
	static void Initialize(); // register callbacks etc.
	explicit WindowEventInterface(const unsigned int&) :
		WindowEventModule(1),
		mModule(nullptr)
	{

	}

	inline void ForwardWindowEvent(std::shared_ptr<WindowEvent> event_ptr)
	{
		// Let module (running on it's own thread) loop through
		// subscribers sending events
		mInstance->mModule->SendToSubscribers(event_ptr);
	}

public:
	static WindowEventInterface* GetInstance();
	static void Bind(GLFWwindow*);
	static void Start();
	static void Stop();
	static void SubscribeModuleToWindowEvents(std::shared_ptr<WindowEventModule>);
	static void MouseClickCB(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCB(GLFWwindow* window, double x, double y);
	static void CharInputCB(GLFWwindow* window, unsigned int text);
	static void KeyPressCB(GLFWwindow* window, int key, int scancode, int action, int mods);
};

WindowEventInterface* WindowEventInterface::mInstance = nullptr;

bool WindowEventInterface::IsModuleLaunched() const
{
	return mInstance->mModule != nullptr && mInstance->mModule->joinable();
}

void WindowEventInterface::Initialize()
{
	// REGISTER ALL INTERAL CALLBACKS
	mInstance->mModule = new WindowEventModule(1);
	mInstance->mModule->RegisterEventHandler(EventName::MOUSE_CLICK, &WindowEventInterface::ForwardWindowEvent);
	mInstance->mModule->RegisterEventHandler(EventName::CURSOR_POS, &WindowEventInterface::ForwardWindowEvent);
	mInstance->mModule->RegisterEventHandler(EventName::TEXT_INPUT, &WindowEventInterface::ForwardWindowEvent);
	mInstance->mModule->RegisterEventHandler(EventName::KEY_PRESS, &WindowEventInterface::ForwardWindowEvent);

}


WindowEventInterface* WindowEventInterface::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new WindowEventInterface(1);
		mInstance->Initialize();
	}
	return mInstance;
}

void WindowEventInterface::Bind(GLFWwindow* window)
{
	// BIND STATIC CALLBACKS
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, WindowEventInterface::MouseClickCB);
	glfwSetCursorPosCallback(window, WindowEventInterface::CursorPosCB);
	glfwSetCharCallback(window, WindowEventInterface::CharInputCB);
	glfwSetKeyCallback(window, WindowEventInterface::KeyPressCB);

}
void WindowEventInterface::Start()
{
	GetInstance()->mModule->Launch();
}

void WindowEventInterface::Stop()
{
	
	if (GetInstance()->IsModuleLaunched())
	{
		WindowEventInterface* controller = GetInstance();
		GetInstance()->mModule->AddEvent(std::make_shared<WindowEvent>(EventName::SHUTDOWN_WINDOW_EVENT, nullptr));
		GetInstance()->mModule->join();
	}
}

void WindowEventInterface::SubscribeModuleToWindowEvents(std::shared_ptr<WindowEventModule> module_ptr)
{
	GetInstance()->mModule->AddSubscriber(module_ptr);
}

void WindowEventInterface::MouseClickCB(GLFWwindow* window, int button, int action, int mods)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::MOUSE_CLICK, window);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_BUTTON, button);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_ACTION, action);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_MODS, mods);
	mInstance->mModule->AddEvent(event_ptr);
}

void WindowEventInterface::CursorPosCB(GLFWwindow* window, double x, double y)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::CURSOR_POS, window);
	event_ptr->Set<double>(EventName::CURSOR_POS_X, x);
	event_ptr->Set<double>(EventName::CURSOR_POS_Y, y);
	mInstance->mModule->AddEvent(event_ptr);
}

void WindowEventInterface::CharInputCB(GLFWwindow* window, unsigned int text)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::TEXT_INPUT, window);
	event_ptr->Set<unsigned int>(EventName::TEXT_INPUT_CHAR, text);
	mInstance->mModule->AddEvent(event_ptr);
}

void WindowEventInterface::KeyPressCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::KEY_PRESS, window);
	event_ptr->Set<int>(EventName::KEY_PRESS_KEY, key);
	event_ptr->Set<int>(EventName::KEY_PRESS_SCANCODE, scancode);
	event_ptr->Set<int>(EventName::KEY_PRESS_ACTION, action);
	event_ptr->Set<int>(EventName::KEY_PRESS_MODS, mods);
	mInstance->mModule->AddEvent(event_ptr);
}

#endif