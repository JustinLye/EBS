#ifndef WINDOW_CONTROLLER_HEADER
#define WINDOW_CONTROLLER_HEADER
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"graphics/WindowEventModule.h"
struct GLFWwindow;
class WindowController : protected WindowEventModule
{
private:
	
protected:
	static WindowController* mInstance;
	WindowEventModule* mModule;

	bool IsModuleLaunched() const;
	static void Initialize(); // register callbacks etc.
	void EHMouseClickCB(std::shared_ptr<WindowEvent>);
	
	explicit WindowController(const unsigned int&) :
		WindowEventModule(1),
		mModule(nullptr)
	{

	}

public:


	template<class T>
	void AddSubscriber(std::shared_ptr<T> subscriber)
	{
		mModule->AddSubscriber(subscriber);
	}
	static WindowController* GetInstance();
	static void Bind(GLFWwindow*);
	static void Start();
	static void Stop();
	static void MouseClickCB(GLFWwindow*, int, int, int);
	
};

WindowController* WindowController::mInstance = nullptr;

bool WindowController::IsModuleLaunched() const
{
	return GetInstance()->mModule->joinable();
}

void WindowController::Initialize()
{
	mInstance->mModule = new WindowEventModule(1);
	mInstance->mModule->RegisterEventHandler(EventName::MOUSE_CLICK, &WindowController::EHMouseClickCB);
}

void WindowController::EHMouseClickCB(std::shared_ptr<WindowEvent> event_ptr)
{
	std::cout << "Button: " << event_ptr->Get<int>(EventName::MOUSE_CLICK_BUTTON) << '\n';
}

WindowController* WindowController::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new WindowController(1);
		mInstance->Initialize();
	}
	return mInstance;
}

void WindowController::Bind(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, WindowController::MouseClickCB);

}
void WindowController::Start()
{
	GetInstance()->mModule->Launch();
}

void WindowController::Stop()
{
	
	if (GetInstance()->IsModuleLaunched())
	{
		WindowController* controller = GetInstance();
		controller->mModule->AddEvent(std::make_shared<WindowEvent>(EventName::SHUTDOWN_WINDOW_EVENT, nullptr));
		controller->mModule->join();
	}
}

void WindowController::MouseClickCB(GLFWwindow* window, int button, int action, int mods)
{
	auto event_ptr = std::make_shared<WindowEvent>(EventName::MOUSE_CLICK, window);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_BUTTON, button);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_ACTION, action);
	event_ptr->Set<int>(EventName::MOUSE_CLICK_MODS, mods);
	mInstance->mModule->AddEvent(event_ptr);
}


#endif