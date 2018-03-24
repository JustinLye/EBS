#include"graphics/AppController.h"

AppController::AppController() :
	mWindowController(nullptr),
	mWindowPtr(nullptr)
{

}

void AppController::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_NONE);
	mWindowPtr = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (mWindowPtr == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(mWindowPtr);
	WindowEventInterface::Start();
	WindowEventInterface::Bind(mWindowPtr);
	mWindowController = std::make_shared<WindowController>();
	std::cout << __FUNCTION__ << " mWindowController ModuleId " << mWindowController->GetId() << '\n';
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, 800, 600);
}

void AppController::EventLoop()
{
	while (!glfwWindowShouldClose(mWindowPtr))
	{
		glfwSwapBuffers(mWindowPtr);
		glfwPollEvents();
	}
}

void AppController::Start()
{
	Initialize();
	EventLoop();
	Shutdown();
}

void AppController::Shutdown()
{
	glfwTerminate();
	WindowEventInterface::Stop();
}

GLFWwindow* AppController::CreateSharedContext()
{
	if (mWindowPtr == NULL)
	{
		return NULL;
	}
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	return glfwCreateWindow(800, 600, "Hidden window", NULL, mWindowPtr);
}

GLFWwindow* AppController::GetWindowPtr()
{
	return mWindowPtr;
}

void AppController::SubscribeToWindowEvents(std::shared_ptr<BaseModule> module_ptr)
{
	WindowEventInterface::SubscribeModuleToWindowEvents(module_ptr);
}

void AppController::SendOneOffEvent(std::shared_ptr<BaseEvent> event_ptr)
{
	WindowEventInterface::SendOneOffEvent(event_ptr);
}