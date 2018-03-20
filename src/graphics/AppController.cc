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
	mWindowPtr = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (mWindowPtr == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	WindowEventInterface::Start();
	WindowEventInterface::Bind(mWindowPtr);
	mWindowController = std::make_shared<WindowController>();
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

GLFWwindow* AppController::GetWindowPtr()
{
	return mWindowPtr;
}