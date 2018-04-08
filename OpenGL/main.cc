#define _USE_MATH_DEFINES
#include<iostream>
#include<chrono>
#include<thread>
#include<algorithm>
#include<queue>
#include<deque>
#include<list>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"graphics/objects/Ship.h"
#include"graphics/objects/Laser.h"

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
const int WINDOW_HEIGHT = 960;
const int WINDOW_WIDTH = 1280;
const std::string VERTEX_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.vert";
const std::string RIPPLE_VERTEX_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\ripple_shader.vert";
const std::string FRAGMENT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.frag";
const std::string GEOMETRY_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.geom";
const std::string TEXTURE_PLANE_FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\texture_plane.frag";
const std::string TEXTURE_PLANE_VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\texture_plane.vert";



//ripple displacement speed
const float SPEED = 1;

//camera transformation variables
int state = 0, oldX = 0, oldY = 0;
float rX = 0, rY = 0, fov = 45.0f;

//current time
//float time = 0;
float frequency = 0;
float amplitude = 0;
int subdivisions = 1;

const unsigned int MAX_LASER_COUNT = 12;
Laser* ActiveLaserArray[MAX_LASER_COUNT];
std::queue<Laser*> LaserQueue;
std::queue<unsigned int> AvailableLaserIndexQueue;
std::list<unsigned int> ActiveLaserIndexList;
float laser_timeout_ms = 128;
std::chrono::time_point<std::chrono::high_resolution_clock> last_laser;
GLFWwindow* InitOpenGL();
void UpdateActiveLasers(const float&);
void InitLaserArray();
void HandleMouseMove(GLFWwindow*, double, double);
void HandleMouseDown(GLFWwindow*, int, int, int);
void HandleResize(GLFWwindow*, int, int);
void HandleKeyPress(GLFWwindow*, int, int, int, int);
void AdditionalInputHandler(GLFWwindow*);
Ship* ship;
int main(int argc, char* argv[])
{
	try
	{
		GLFWwindow* window = InitOpenGL();
		glfwSetCursorPosCallback(window, HandleMouseMove);
		glfwSetMouseButtonCallback(window, HandleMouseDown);
		glfwSetWindowSizeCallback(window, HandleResize);
		glfwSetKeyCallback(window, HandleKeyPress);
		
		ship = new Ship();
		double old_time = glfwGetTime();
		
		long target_frame_rate_ms = 16;
		auto start_time = std::chrono::high_resolution_clock::now();
		auto last_laser = std::chrono::high_resolution_clock::now() - std::chrono::duration<double,std::milli>(laser_timeout_ms);
		InitLaserArray();
		while (!glfwWindowShouldClose(window))
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
			//time = (glfwGetTime() - old_time) / glfwGetTimerFrequency();
			//old_time = time;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			AdditionalInputHandler(window);
			UpdateActiveLasers(1.0f/60.0f);
			glm::mat4 m = glm::mat4(1.0f);
			ship->Update(1.0f/60.0f);
			if (ship->mTranslate.x > 0.0001f || ship->mTranslate.x < -0.0001f)
			{
				m = glm::translate(m, ship->mTranslate);
			}
			ship->Render(glm::value_ptr(m));
			glfwSwapBuffers(window);
			glfwPollEvents();
			std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now() - start_time;
			std::this_thread::sleep_for(std::chrono::milliseconds((long)(target_frame_rate_ms - elapsed.count())));
		}
	}
	catch (std::exception& error)
	{
		std::cout << error.what() << '\n';
	}
	

}

void InitLaserArray()
{
	for (int i = 0; i < MAX_LASER_COUNT; ++i)
	{
		ActiveLaserArray[i] = new Laser();
		AvailableLaserIndexQueue.push(i);
	}
}

GLFWwindow* InitOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ripple", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return window;
}

void HandleMouseMove(GLFWwindow* window, double x, double y)
{
	/*if (state == 0)
	{
		fov += (y - oldY) * time;
		camera.Projection(fov , camera.AspectRatio());
	}
	else
	{
		rY += (x - oldX) * (time);
		rX += (y - oldY) * (time);
		camera.Rotate(rX, rY, 0);
	}
	oldX = x;
	oldY = y;*/
}

void HandleMouseDown(GLFWwindow* window, int button, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		oldX = xpos;
		oldY = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		state = 0;
	}
	else
	{
		state = 1;
	}
}

void HandleResize(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void UpdateActiveLasers(const float& dt)
{
	// Update all active lasers
	glm::mat4 m = glm::mat4(1.0f);
	glm::mat4 mi = glm::mat4(1.0f);

	std::list<unsigned int>::iterator active_index_iter = ActiveLaserIndexList.begin();
	while (active_index_iter != ActiveLaserIndexList.end())
	{
		Laser* laser = ActiveLaserArray[*active_index_iter];
		laser->Update(dt);
		if (laser->mTranslate.y > 2.20f)
		{
			AvailableLaserIndexQueue.push(*active_index_iter);
			active_index_iter = ActiveLaserIndexList.erase(active_index_iter);
		}
		else
		{
			m = glm::translate(mi, laser->mTranslate);
			laser->Render(glm::value_ptr(m));
			active_index_iter++;
		}
	}	
}

void AdditionalInputHandler(GLFWwindow* window)
{
	bool strafe_right = (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE);
	bool strafe_left = (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE);
	bool req_to_fire = (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_RELEASE);
	if (strafe_right)
	{
		ship->Strafe(Ship::DIRECTION::RIGHT);
	}
	if (strafe_left)
	{
		ship->Strafe(Ship::DIRECTION::LEFT);
	}
	if (req_to_fire)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_laser).count() >= laser_timeout_ms)
		{
			if (!AvailableLaserIndexQueue.empty())
			{
				unsigned int i = AvailableLaserIndexQueue.front();
				AvailableLaserIndexQueue.pop();
				ActiveLaserIndexList.push_front(i);
				ActiveLaserArray[i]->mTranslate.y = 0.25f; // moves y the nose of ship (-1.0f + 0.25 = -0.75f)
				ActiveLaserArray[i]->mTranslate.x = ship->mTranslate.x; // moves x to center of ship
				last_laser = std::chrono::high_resolution_clock::now();
			}
		}
	}
}

void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	}

}