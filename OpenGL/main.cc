#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//#include"util/Singleton.h"
//#include"graphics/WindowController.h"
#include"graphics/AppController.h"
#include"util/FileReader.h"
#include"graphics/VertexBufferEventModule.h"
#include"graphics/BaseShapeModule.h"

const char* VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.vert";
const char* FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.frag";

class TestWindowCtlr :
	public WindowEventInterface
{
public:

};

struct TestShapeWrapper
{
	std::shared_ptr<VertexBufferEventModule> vbuff_ptr;
	std::shared_ptr<BaseShapeModule> shp_mod_ptr;
	TestShapeWrapper() :
		vbuff_ptr(std::make_shared<VertexBufferEventModule>()),
		shp_mod_ptr(std::make_shared<BaseShapeModule>(vbuff_ptr))
	{
		vbuff_ptr->AddSubscriber(shp_mod_ptr);
		vbuff_ptr->Launch();
		shp_mod_ptr->Launch();
	}
	GLuint GetVAO()
	{
		return shp_mod_ptr->GetVAO();
	}
	void Shutdown()
	{
		if (shp_mod_ptr->joinable())
		{
			shp_mod_ptr->AddEvent(MakeVBuffEventPtr(EventName::VBUFF_SHUTDOWN, shp_mod_ptr->GetId()));
			shp_mod_ptr->join();
		}
		if (vbuff_ptr->joinable())
		{
			vbuff_ptr->AddEvent(MakeVBuffEventPtr(EventName::VBUFF_SHUTDOWN, vbuff_ptr->GetId()));
			vbuff_ptr->join();
		}
	}
	~TestShapeWrapper()
	{
		Shutdown();
	}
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

	TestShapeWrapper shape_wrapper;

	unsigned int VBO, VBO2, VAO2;
	//unsigned int VAO = 0;
	//shape_wrapper.GetVAO();
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//VAO = shape_wrapper.GetVAO();
	std::cout << "shape_wrapper VAO ";
	std::cout << shape_wrapper.GetVAO() << '\n';
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	std::cout << 'VAO ' << VAO << '\n';
	//std::cout << "Shape Wrapper VAO " << shape_wrapper.GetVAO() << '\n';
	glGenVertexArrays(1, &VAO2);
	std::cout << "VAO2 " << VAO2 << '\n';
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_prog);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteVertexArrays(1, &shape_wrapper.GetVAO());
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO);
	shape_wrapper.Shutdown();
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