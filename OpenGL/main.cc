#define _USE_MATH_DEFINES
#include<iostream>
#include<chrono>
#include<thread>
#include<algorithm>
#include<queue>
#include<deque>
#include<list>
#include<random>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<boost/lexical_cast.hpp>

#include"graphics/objects/Ship.h"
#include"entities/LaserCannon.h"
#include"entities/AstroidManager.h"
#include"graphics/geometry/Sphere.h"
#ifdef FONT_DEBUG
#include"graphics/objects/Canvas.h"
#endif
#ifdef COLLISION_DEBUG
class TestSphere :
	public Sphere
{
public:
	TestSphere() :
		Sphere()
	{
		Init();
		mShader.LoadFromFile(ShaderProgram::VERTEX, "C:\\EBS\\OpenGL\\shaders\\sphere.vert");
		mShader.LoadFromFile(ShaderProgram::FRAGMENT, "C:\\EBS\\OpenGL\\shaders\\sphere.frag");
		mShader.CreateAndLink();
		mShader.Use();
		mShader.AddAttribute("vVertex");
		mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\sphere.vert");
		mShader.AutoFillUniformsFromFile("C:\\EBS\\OpenGL\\shaders\\sphere.frag");
		mShader.UnUse();
	}

	float GetRadius()
	{
		return 0.085f;
	}
	glm::vec3 GetOrigin()
	{
		return glm::vec3(0.0f, -0.85f, 0.0f);
	}
};
#endif

const int WINDOW_HEIGHT = 960;
const int WINDOW_WIDTH = 1280;

#ifdef FONT_DEBUG
class TestFont :
	public Font
{
public:
	TestFont() :
		Font()
	{
		Init();
	}
	virtual ~TestFont()
	{

	}
	const char* PathToFont()
	{
		return "C:\\EBS\\OpenGL\\fonts\\arial.ttf";
	}
	int FontWidth()
	{
		return 0;
	}
	int FontHeight()
	{
		return 48;
	}
};

class TestCanvas :
	public Canvas
{
public:
	TestCanvas() :
		Canvas()
	{

	}
	~TestCanvas()
	{

	}
	float ScreenWidth()
	{
		return WINDOW_WIDTH;
	}
	float ScreenHeight()
	{
		return WINDOW_HEIGHT;
	}
};

#endif

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

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

const unsigned int AMMO_CAP = 7;
const unsigned int SHOT_TIMEOUT = 80;
const float TERM_Y_VAL = 2.20f;
LaserCannon* cannon;
AstroidManager* astroid_mgr;
#ifdef COLLISION_DEBUG
TestSphere* test_sphere;
void DoCollisionDetection();
#endif
#ifdef FONT_DEBUG
TestFont* test_font;
TestCanvas* test_canvas;
#endif
GLFWwindow* InitOpenGL();
unsigned int astroids_destroyed = 0;
const std::string SCORE_TEXT = "Astroids Destroyed: ";
#define GetScoreText(score) SCORE_TEXT + boost::lexical_cast<std::string>(score)
void HandleResize(GLFWwindow*, int, int);
void HandleKeyPress(GLFWwindow*, int, int, int, int);
void AdditionalInputHandler(GLFWwindow*);
Ship* ship;
int main(int argc, char* argv[])
{
	try
	{
		GLFWwindow* window = InitOpenGL();
		glfwSetWindowSizeCallback(window, HandleResize);
		glfwSetKeyCallback(window, HandleKeyPress);
		
		ship = new Ship();
#ifdef COLLISION_DEBUG
		test_sphere = new TestSphere();
		
#endif
#ifdef FONT_DEBUG
		//test_font = new TestFont();
		test_canvas = new TestCanvas();
		test_canvas->mFont = std::shared_ptr<Font>(new TestFont());
		test_canvas->Init();
#endif
		double old_time = glfwGetTime();
		cannon = new LaserCannon(AMMO_CAP, SHOT_TIMEOUT, TERM_Y_VAL);
		long target_frame_rate_ms = 16;
		auto start_time = std::chrono::high_resolution_clock::now();
		float orig_line_width;
		glGetFloatv(GL_LINE_WIDTH, &orig_line_width);
		float line_width = 0.55f;
		astroid_mgr = new AstroidManager();
		while (!glfwWindowShouldClose(window))
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
			//time = (glfwGetTime() - old_time) / glfwGetTimerFrequency();
			//old_time = time;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			AdditionalInputHandler(window);
			astroid_mgr->Update(1.0f / 60.0f);
			glLineWidth(line_width);
			cannon->Update(1.0f / 60.0f);
			glLineWidth(orig_line_width);
			glm::mat4 m = glm::mat4(1.0f);
			ship->Update(1.0f/60.0f);
			if (ship->mTranslate.x > 0.0001f || ship->mTranslate.x < -0.0001f)
			{
				m = glm::translate(m, ship->mTranslate);
			}
			astroid_mgr->Render();
			cannon->Render();
			ship->Render(glm::value_ptr(m));
			test_canvas->Render(GetScoreText(astroids_destroyed), 25.0f, WINDOW_HEIGHT-25.0f, 0.25f, glm::vec3(0.5f, 0.8f, 0.2f));
#ifdef COLLISION_DEBUG
			test_sphere->Render(glm::value_ptr(m));
			DoCollisionDetection();
#endif
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


void HandleResize(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
		cannon->Fire(glm::vec3(ship->mTranslate.x, 0.25f, 0.0f));
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

#ifdef COLLISION_DEBUG
void DoCollisionDetection()
{
	std::list<unsigned int>::iterator astroid_iter = astroid_mgr->mActiveIndexList.begin();
	while (astroid_iter != astroid_mgr->mActiveIndexList.end())
	{
		std::list<unsigned int>::iterator laser_iter = cannon->mActiveIndexList.begin();
		Astroid* astroid = astroid_mgr->mAstroidArray[*astroid_iter];
		bool destroy_astroid = false;
		while (laser_iter != cannon->mActiveIndexList.end())
		{
			Laser* laser = cannon->mLaserMag[*laser_iter];
			if (astroid->mCollider.CollisionDetected(laser->mCollider))
			{
				++astroids_destroyed;
				astroid->Color(glm::vec4(std::min(1.0f,astroid->Color().x+0.10f), 0.0f, 0.0f, 0.5f));
				if (astroid->Speed() < 3.0f)
				{
					astroid->Speed(astroid->Speed()*1.25);
				}
				destroy_astroid = true;
				cannon->mAvailableIndexQueue.push(*laser_iter);
				laser_iter = cannon->mActiveIndexList.erase(laser_iter);
				std::cout << "collision detected\n";
				break;
			}
			else
			{
				++laser_iter;
			}
		}
		if (destroy_astroid)
		{
			astroid_mgr->mAvailableIndexQueue.push(*astroid_iter);
			astroid_iter = astroid_mgr->mActiveIndexList.erase(astroid_iter);
		}
		else
		{
			++astroid_iter;
		}
	}
}
#endif