#include<iostream>
#include<algorithm>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"graphics/shader/ShaderProgram.h"
#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
const int WINDOW_HEIGHT = 960;
const int WINDOW_WIDTH = 1280;
const std::string VERTEX_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.vert";
const std::string RIPPLE_VERTEX_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\ripple_shader.vert";
const std::string FRAGMENT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.frag";
const std::string GEOMETRY_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\shader.geom";

//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

const int NUM_X = 40; //total quads on X axis
const int NUM_Z = 40; //total quads on Z axis

const float SIZE_X = 4; //size of plane in world space
const float SIZE_Z = 4;
const float HALF_SIZE_X = SIZE_X / 2.0f;
const float HALF_SIZE_Z = SIZE_Z / 2.0f;

//ripple displacement speed
const float SPEED = 2;

//ripple mesh vertices and indices
glm::vec3 vertices[4];
GLushort indices[6];

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

//camera transformation variables
int state = 0, oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -35;

//current time
float time = 0;
int subdivisions = 1;
ShaderProgram shader_program;
GLFWwindow* InitOpenGL();

void HandleMouseMove(GLFWwindow*, double, double);
void HandleMouseDown(GLFWwindow*, int, int, int);
void HandleResize(GLFWwindow*, int, int);
void HandleKeyPress(GLFWwindow*, int, int, int, int);
int main(int argc, char* argv[])
{
	try
	{
		GLFWwindow* window = InitOpenGL();
		glfwSetCursorPosCallback(window, HandleMouseMove);
		glfwSetMouseButtonCallback(window, HandleMouseDown);
		glfwSetWindowSizeCallback(window, HandleResize);
		glfwSetKeyCallback(window, HandleKeyPress);
		
		shader_program.LoadFromFile(ShaderProgram::VERTEX, VERTEX_SHADER_PATH);
		shader_program.LoadFromFile(ShaderProgram::FRAGMENT, FRAGMENT_SHADER_PATH);
		shader_program.LoadFromFile(ShaderProgram::GEOMETRY, GEOMETRY_SHADER_PATH);
		shader_program.CreateAndLink();
		shader_program.Use();
		shader_program.AddAttribute("vVertex");
		shader_program.AutoFillUniformsFromFile(GEOMETRY_SHADER_PATH);
		shader_program.UnUse();
		//setup plane geometry
		//setup plane vertices

		vertices[0] = glm::vec3(-5, 0, -5);
		vertices[1] = glm::vec3(-5, 0, 5);
		vertices[2] = glm::vec3(5, 0, 5);
		vertices[3] = glm::vec3(5, 0, -5);

		GLushort* id = &indices[0];

		*id++ = 0;
		*id++ = 1;
		*id++ = 2;
		*id++ = 0;
		*id++ = 2;
		*id++ = 3;


		GL_CHECK_ERRORS
		//setup plane vao and vbo stuff
		glGenVertexArrays(1, &vaoID);
		glGenBuffers(1, &vboVerticesID);
		glGenBuffers(1, &vboIndicesID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
		//pass plane vertices to array buffer object
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
			//enable vertex attrib array for position
			glEnableVertexAttribArray(shader_program["vVertex"]);
		glVertexAttribPointer(shader_program["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		GL_CHECK_ERRORS
			//pass the plane indices to element array buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
			double old_time = glfwGetTime();
		int maxv;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxv);
		std::cout << "max out verts " << maxv << '\n';
		while (!glfwWindowShouldClose(window))
		{
			time = (glfwGetTime()-old_time) * SPEED;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
			glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
			MV = glm::translate(MV, glm::vec3(-5.0f, 0, -5.0f));
			shader_program.Use();
			glUniform1i(shader_program("sub_divisions"), subdivisions);
			glUniformMatrix4fv(shader_program("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
			MV = glm::translate(MV, glm::vec3(10.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(shader_program("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
			MV = glm::translate(MV, glm::vec3(0.0f, 0.0f, 10.0f));
			glUniformMatrix4fv(shader_program("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
			MV = glm::translate(MV, glm::vec3(-10.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(shader_program("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
			shader_program.UnUse();
			glfwSwapBuffers(window);
			glfwPollEvents();
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
	P = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);
	return window;
}

void HandleMouseMove(GLFWwindow* window, double x, double y)
{
	if (state == 0)
	{
		dist *= (1 + (y - oldY) * (time / 1000.0f));
	}
	else
	{
		rY += (x - oldX) * (time / 1000.0f);
		rX += (y - oldY) * (time / 1000.0f);
	}
	oldX = x;
	oldY = y;
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
	P = glm::perspective(45.0f, (GLfloat)w / h, 1.0f, 1000.0f);
}

void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	switch (action)
	{
	case GLFW_REPEAT:
	case GLFW_PRESS:
		switch (key)
		{
		case GLFW_KEY_UP:
			subdivisions++;
			subdivisions = std::max(1, std::min(8, subdivisions));
			break;
		case GLFW_KEY_DOWN:
			subdivisions--;
			subdivisions = std::max(1, std::min(8, subdivisions));
			break;
		case GLFW_KEY_LEFT:
			break;
		case GLFW_KEY_RIGHT:
			break;
		case GLFW_KEY_PAGE_UP:
			break;
		case GLFW_KEY_PAGE_DOWN:	
			break;
		}
		break;
	default:
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}
		break;
	}
}