#include<iostream>
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
glm::vec3 vertices[(NUM_X + 1)*(NUM_Z + 1)];
const int TOTAL_INDICES = NUM_X*NUM_Z * 2 * 3;
GLushort indices[TOTAL_INDICES];

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

//camera transformation variables
int state = 0, oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -7;

//current time
float time = 0;
float amplitude = 0.125;
float frequency = 4;
float pi_coeff = -1.0f;
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
		
		shader_program.LoadFromFile(ShaderProgram::VERTEX, RIPPLE_VERTEX_SHADER_PATH);
		shader_program.LoadFromFile(ShaderProgram::FRAGMENT, FRAGMENT_SHADER_PATH);
		shader_program.CreateAndLink();
		shader_program.Use();
		shader_program.AddAttribute("vVertex");
		/*shader_program.AddUniform("MVP");
		shader_program.AddUniform("time");
		shader_program.AddUniform("amplitude");
		shader_program.AddUniform("frequency");
		shader_program.AddUniform("pi_coeff");*/
		shader_program.AutoFillUniformsFromFile(RIPPLE_VERTEX_SHADER_PATH);
		shader_program.UnUse();
		//setup plane geometry
		//setup plane vertices
		int count = 0;
		int i = 0, j = 0;
		for (j = 0; j <= NUM_Z; j++) {
			for (i = 0; i <= NUM_X; i++) {
				vertices[count++] = glm::vec3(((float(i) / (NUM_X - 1)) * 2 - 1)* HALF_SIZE_X, 0, ((float(j) / (NUM_Z - 1)) * 2 - 1)*HALF_SIZE_Z);
			}
		}

		//fill plane indices array
		GLushort* id = &indices[0];
		for (i = 0; i < NUM_Z; i++) {
			for (j = 0; j < NUM_X; j++) {
				int i0 = i * (NUM_X + 1) + j;
				int i1 = i0 + 1;
				int i2 = i0 + (NUM_X + 1);
				int i3 = i2 + 1;
				if ((j + i) % 2) {
					*id++ = i0; *id++ = i2; *id++ = i1;
					*id++ = i1; *id++ = i2; *id++ = i3;
				} else {
					*id++ = i0; *id++ = i2; *id++ = i3;
					*id++ = i0; *id++ = i3; *id++ = i1;
				}
			}
		}

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
		while (!glfwWindowShouldClose(window))
		{
			time = (glfwGetTime()-old_time) * SPEED;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
			glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 MVP = P*MV;
			shader_program.Use();
			glUniformMatrix4fv(shader_program("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniform1f(shader_program("time"), time);
			glUniform1f(shader_program("amplitude"), amplitude);
			glUniform1f(shader_program("frequency"), frequency);
			glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
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
	return window;
}

void HandleMouseMove(GLFWwindow* window, double x, double y)
{
	if (state == 0)
	{
		dist *= (1 + (y - oldY) / 60.0f);
	}
	else
	{
		rY += (x - oldX) / 5.0f;
		rX += (y - oldY) / 5.0f;
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
			amplitude *= 1.10f;
			std::cout << "amplitude: " << amplitude << '\n';
			break;
		case GLFW_KEY_DOWN:
			amplitude *= 0.90f;
			std::cout << "amplitude: " << amplitude << '\n';
			break;
		case GLFW_KEY_LEFT:
			frequency *= 0.90f;
			std::cout << "frequency: " << frequency << '\n';
			break;
		case GLFW_KEY_RIGHT:
			frequency *= 1.10f;
			std::cout << "frequency: " << frequency << '\n';
			break;
		case GLFW_KEY_PAGE_UP:
			pi_coeff += 0.25f;
			std::cout << "pi_coeff: " << pi_coeff << '\n';
			break;
		case GLFW_KEY_PAGE_DOWN:	
			pi_coeff -= 0.25f;
			std::cout << "pi_coeff: " << pi_coeff << '\n';
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