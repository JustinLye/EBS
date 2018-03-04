#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"graphics/WindowEvent.h"
struct Foo
{
	static void MouseButtonCB(GLFWwindow*, int, int, int);
};

void MouseButtonCB(GLFWwindow*, int, int, int);
int main(int argc, char* argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, Foo::MouseButtonCB);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return EXIT_SUCCESS;
}

void Foo::MouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
	std::string button_str = button == 0 ? "Left-Button " : "Right-Button ";
	std::string action_str = action == 1 ? "pressed" : "released";
	std::cout << button_str.c_str() << action_str.c_str() << '\n';
}

void MouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Button: " << button << '\n'
		<< "Action: " << action << '\n'
		<< "Mods: " << mods << '\n';
}
