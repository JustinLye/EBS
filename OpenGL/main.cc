#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//#include"util/Singleton.h"
//#include"graphics/WindowController.h"
#include"graphics/AppController.h"
#include"util/FileReader.h"
#include"graphics/shape/BaseShape.h"

const char* VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.vert";
const char* FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.frag";

class TestWindowCtlr :
	public WindowEventInterface
{
public:

};


int CreateShaderProg();
int main(int argc, char* argv)
{
	AppController app;
	app.Initialize();
	GLFWwindow* window = app.GetWindowPtr();
	int shader_prog;
	try
	{
		shader_prog = CreateShaderProg();

	}
	catch (std::exception& error)
	{
		std::cout << error.what() << '\n';
	}
	float verts[] =
	{
		-0.9f, -0.9f, 0.0f,
		-0.4f, -0.9f, 0.0f,
		-0.65f, -0.2f, 0.0f
	};
	float verts2[] =
	{
		0.9f, 0.2f, 0.0f,
		0.4f, 0.2f, 0.0f,
		0.65f, 0.9f, 0.0f
	};

	BaseShape left_tri;
	BaseShape point_data;
	point_data.SetPointData(verts2, 9);
	left_tri.SetPointData(verts, 9);
	point_data.Initialize();
	left_tri.Initialize();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_prog);
		left_tri.Draw();
		point_data.Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	point_data.Shutdown();
	left_tri.Shutdown();
	app.Shutdown();
	
	return EXIT_SUCCESS;
}

int CreateShaderProg()
{
	unsigned int vert_shader, frag_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vert_source_str, frag_source_str;
	FileReader::GetInstance()->FileToString(VERT_SHADER_PATH, vert_source_str);
	FileReader::GetInstance()->FileToString(FRAG_SHADER_PATH, frag_source_str);
	const char* vert_shader_source = vert_source_str.c_str();
	const char* frag_shader_source = frag_source_str.c_str();
	glShaderSource(vert_shader, 1, &vert_shader_source, NULL);
	glCompileShader(vert_shader);
	char info_log[512];
	int success;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
		std::string err_msg = std::string("Vert shader failed to compile. ") + info_log;
		throw std::runtime_error(APP_ERROR_MESSAGE(err_msg.c_str()));
	}
	glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	glCompileShader(frag_shader);
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::string err_msg = std::string("Frag shader failed to compile. ") + info_log;
		throw std::runtime_error(APP_ERROR_MESSAGE(err_msg.c_str()));
	}
	int shader_prog = glCreateProgram();
	glAttachShader(shader_prog, vert_shader);
	glAttachShader(shader_prog, frag_shader);
	glLinkProgram(shader_prog);
	glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_prog, 512, NULL, info_log);
		std::string err_msg = std::string("Shader prog. failed to link. ") + info_log;
		throw std::runtime_error(APP_ERROR_MESSAGE(err_msg.c_str()));
	}
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
	return shader_prog;
}