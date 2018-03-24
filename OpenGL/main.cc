#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//#include"util/Singleton.h"
//#include"graphics/WindowController.h"
#include"graphics/AppController.h"
#include"util/FileReader.h"
#include"graphics/shape/BaseShape.h"
#include"graphics/shape/ShapeEvent.h"
#include"graphics/window/WindowBackground.h"

const char* VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.vert";
const char* FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.frag";
const char* FRAG2_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core2.frag";


class TestModule :
	public BaseModule
{
	TestModule(const TestModule&) {}
public:
	BaseShape mShape;
	bool mDrawIt;
	TestModule() :
		BaseModule(),
		mDrawIt(false)
	{
		std::cout << __FUNCTION__ << '\n';
		RegisterEventHandler(EventName::KEY_PRESS, &TestModule::HandleKeyPress);
		RegisterEventHandler(EventName::MAKE_CONTEXT_CURRENT, &TestModule::HandleContext);
	}
	void HandleKeyPress(std::shared_ptr<WindowEvent> event_ptr)
	{
		std::cout << "key: " << event_ptr->Get<int>(EventName::KEY_PRESS_KEY) << '\t' << "action: " << event_ptr->Get<int>(EventName::KEY_PRESS_ACTION) << '\n';
		std::cout << "minus: " << GLFW_KEY_MINUS << '\n';
		if (event_ptr->Get<int>(EventName::KEY_PRESS_KEY) == GLFW_KEY_MINUS)
		{
			mShape.Scale(0.10f);
		}
		mDrawIt = true;
	}
	void HandleContext(std::shared_ptr<WindowEvent> event_ptr)
	{
		assert(event_ptr->GetWindowPtr() != nullptr);
		glfwMakeContextCurrent(event_ptr->GetWindowPtr());
	}

	void Register()
	{
		
	}
};


int CreateShaderProg(const char*, const char*);
int main(int argc, char* argv)
{
	std::cout << "Main Thread Id " << std::this_thread::get_id() << '\n';
	AppController app;
	app.Initialize();
	GLFWwindow* window = app.GetWindowPtr();
	GLFWwindow* shared_window = app.CreateSharedContext();
	int shader_prog, shader2_prog;
	try
	{
		shader_prog = CreateShaderProg(VERT_SHADER_PATH, FRAG_SHADER_PATH);
		shader2_prog = CreateShaderProg(VERT_SHADER_PATH, FRAG2_SHADER_PATH);

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
	std::shared_ptr<TestModule> test_module = std::make_shared <TestModule>();
	test_module->Launch();
	std::cout << "test_module id " << test_module->GetId() << '\n';
	BaseShape point_data;
	WindowBackground window_back(0.2f, 0.3f, 0.3f, 1.0f);
	point_data.SetPointData(verts2, 9);
	test_module->mShape.SetPointData(verts, 9);
	GLuint vao1, vao2, vbo1, vbo2;
	glGenVertexArrays(1, &vao1);
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	point_data.Initialize(vbo1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	test_module->mShape.Initialize(vbo2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	app.SubscribeToWindowEvents(test_module);
	WindowEventInterface::SendOneOffEvent<WindowEvent>(std::make_shared<WindowEvent>(EventName::MAKE_CONTEXT_CURRENT, shared_window));
	auto start_time = std::chrono::high_resolution_clock::now();


	/*
		You have 2 options
		(1) [prefered] learn about shared objects
		(2) mangage context release and binding between different threads, which means you'll have a linear event loop because you have to wait on each thread to finish with the context.
	*/
	bool scaleit = true;
	while (!glfwWindowShouldClose(window))
	{
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() > 4)
		{
			start_time = std::chrono::high_resolution_clock::now();
			//point_data.Scale(-0.10f);
			test_module->mDrawIt = false;
			scaleit = true;
		}
		window_back.Draw();
		glBindVertexArray(vao2);
		glUseProgram(shader_prog);
		if (test_module->mDrawIt)
		{
			/*if (scaleit)
			{
				test_module->mShape.Scale(0);
				scaleit = false;
			}*/
			//test_module->mDrawIt = false;
			glfwMakeContextCurrent(NULL);
			test_module->AddEvent(std::make_shared<WindowEvent>(EventName::MAKE_CONTEXT_CURRENT, app.GetWindowPtr()));
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			test_module->mShape.Draw();
			test_module->AddEvent(std::make_shared<WindowEvent>(EventName::MAKE_CONTEXT_CURRENT, shared_window));
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			glfwMakeContextCurrent(app.GetWindowPtr());
		}
		glUseProgram(shader2_prog);
		glBindVertexArray(vao1);
		//point_data.Draw();
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	point_data.Shutdown();
	test_module->mShape.Shutdown();
	test_module->AddEvent(std::make_shared<WindowEvent>(EventName::SHUTDOWNEVENT, app.GetWindowPtr()));
	test_module->join();
	app.Shutdown();
	
	return EXIT_SUCCESS;
}

int CreateShaderProg(const char* vert_path, const char* frag_path)
{
	unsigned int vert_shader, frag_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vert_source_str, frag_source_str;
	FileReader::GetInstance()->FileToString(vert_path, vert_source_str);
	FileReader::GetInstance()->FileToString(frag_path, frag_source_str);
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