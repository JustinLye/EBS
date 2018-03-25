#include"graphics/window/WindowEventInterface.h"

WindowEventInterface* WindowEventInterface::mInstance = nullptr;

WindowEventInterface::WindowEventInterface() :
	BaseModule(),
	mModule(nullptr)
{
}

/*void WindowEventInterface::ForwardWindowEvent(std::shared_ptr<WindowEvent> event_ptr)
{
	// Let module (running on it's own thread) loop through
	// subscribers sending events
	mInstance->mModule->SendToSubscribers(event_ptr);
}*/

bool WindowEventInterface::IsModuleLaunched() const
{
	return mInstance->mModule != nullptr && mInstance->mModule->joinable();
}

void WindowEventInterface::Initialize()
{
	// REGISTER ALL INTERAL CALLBACKS
	mInstance->mModule = new BaseModule;
	mInstance->mModule->RegisterEventHandler(EventName::MOUSE_CLICK, &WindowEventInterface::ForwardWindowEvent<WindowEvent>);
	mInstance->mModule->RegisterEventHandler(EventName::CURSOR_POS, &WindowEventInterface::ForwardWindowEvent<WindowEvent>);
	mInstance->mModule->RegisterEventHandler(EventName::TEXT_INPUT, &WindowEventInterface::ForwardWindowEvent<WindowEvent>);
	mInstance->mModule->RegisterEventHandler(EventName::KEY_PRESS, &WindowEventInterface::ForwardWindowEvent<WindowEvent>);

}


WindowEventInterface* WindowEventInterface::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new WindowEventInterface;
		mInstance->Initialize();
	}
	return mInstance;
}

void WindowEventInterface::Bind(GLFWwindow* window)
{
	// BIND STATIC CALLBACKS
	// Assumes window (context) is current
	glfwSetMouseButtonCallback(window, WindowEventInterface::MouseClickCB);
	glfwSetCursorPosCallback(window, WindowEventInterface::CursorPosCB);
	glfwSetCharCallback(window, WindowEventInterface::CharInputCB);
	glfwSetKeyCallback(window, WindowEventInterface::KeyPressCB);

}
void WindowEventInterface::Start()
{
	if (!GetInstance()->IsModuleLaunched())
	{
		GetInstance()->mModule->Launch();
	}
}

void WindowEventInterface::Stop()
{

	if (GetInstance()->IsModuleLaunched())
	{
		WindowEventInterface* controller = GetInstance();
		GetInstance()->mModule->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
		GetInstance()->mModule->join();
	}
}

/*void WindowEventInterface::SubscribeModuleToWindowEvents(std::shared_ptr<WindowEventModule> module_ptr)
{
	GetInstance()->mModule->AddSubscriber(module_ptr);
}*/

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

void WindowEventInterface::CursorEnterCB(GLFWwindow* window, int entered)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::CURSOR_ENTER, window);
	event_ptr->Set<int>(EventName::CURSOR_ENTER_VAL, entered);
	mInstance->mModule->AddEvent(event_ptr);
}

void WindowEventInterface::MouseScrollCB(GLFWwindow* window, double xoffset, double yoffset)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::MOUSE_SCROLL, window);
	event_ptr->Set<double>(EventName::MOUSE_SCROLL_X, xoffset);
	event_ptr->Set<double>(EventName::MOUSE_SCROLL_Y, yoffset);
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

void WindowEventInterface::SubscribeModuleToWindowEvents(std::shared_ptr<BaseModule> module_ptr)
{
	GetInstance()->mModule->AddSubscriber(module_ptr);
}