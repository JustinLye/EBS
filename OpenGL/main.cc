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
#include"graphics/shape/ShapeModule.h"

const char* VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.vert";
const char* FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.frag";
const char* FRAG2_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core2.frag";



class RenderModule :
	public BaseModule
{
protected:
	bool mRenderComplete;
	bool mContextSwitchComplete;
	std::mutex mRenderMtx;
	std::mutex mContextMtx;
	std::condition_variable mRenderCv;
	std::condition_variable mContextCv;

public:
	std::shared_ptr<ShapeModule> mShapeModule;
	RenderModule();
	void TakeControlOfContext(GLFWwindow*);
	void ReleaseControlOfContext(GLFWwindow*);
	void HandleRenderAckEvent(std::shared_ptr<BaseEvent>);
	void HandleContextAckEvent(std::shared_ptr<BaseEvent>);
	void Render(GLFWwindow*);
	void AddShape(GLFWwindow*, std::shared_ptr<ShapeEvent>);
};

void RenderModule::TakeControlOfContext(GLFWwindow* window)
{
	std::unique_lock<std::mutex> locker(mContextMtx);
	mContextSwitchComplete = false;
	std::shared_ptr<WindowEvent> control_context_event_ptr = std::make_shared<WindowEvent>(EventName::RENDER_CONTEXT_CHANGE_CONTROL, window);
	glfwMakeContextCurrent(NULL);
	mShapeModule->AddEvent(control_context_event_ptr);
	while (!mContextSwitchComplete)
	{
		mContextCv.wait(locker);
	}


}

void RenderModule::ReleaseControlOfContext(GLFWwindow* window)
{
	std::unique_lock<std::mutex> locker(mContextMtx);
	mContextSwitchComplete = false;
	std::shared_ptr<WindowEvent> release_context_event_ptr = std::make_shared<WindowEvent>(EventName::RENDER_CONTEXT_CHANGE_RELEASE, window);
	mShapeModule->AddEvent(release_context_event_ptr);
	while (!mContextSwitchComplete)
	{
		mContextCv.wait(locker);
	}
	glfwMakeContextCurrent(window);
}

RenderModule::RenderModule() :
	BaseModule(),
	mRenderComplete(false),
	mContextSwitchComplete(false),
	mShapeModule(std::make_shared<ShapeModule>())
{
	RegisterEventHandler(EventName::RENDER_DRAWING_COMPLETE, &RenderModule::HandleRenderAckEvent);
	RegisterEventHandler(EventName::SHAPE_ADDSHAPE_COMPLETE, &RenderModule::HandleRenderAckEvent);
	RegisterEventHandler(EventName::RENDER_CONTEXT_CHANGE_COMPLETE, &RenderModule::HandleContextAckEvent);
}

void RenderModule::HandleRenderAckEvent(std::shared_ptr<BaseEvent> event_ptr)
{
	std::unique_lock<std::mutex> locker(mRenderMtx);
	mRenderComplete = true;
	mRenderCv.notify_all();
}

void RenderModule::HandleContextAckEvent(std::shared_ptr<BaseEvent> event_ptr)
{
	std::unique_lock<std::mutex> locker(mContextMtx);
	mContextSwitchComplete = true;
	mContextCv.notify_all();
}

void RenderModule::Render(GLFWwindow* window)
{
	std::unique_lock<std::mutex> locker(mRenderMtx);
	mRenderComplete = false;
	TakeControlOfContext(window);
	std::shared_ptr<BaseEvent> draw_event_ptr = std::make_shared<BaseEvent>(EventName::RENDER_DRAW);
	mShapeModule->AddEvent(draw_event_ptr);
	while (!mRenderComplete)
	{
		mRenderCv.wait(locker);
	}
	ReleaseControlOfContext(window);
	
}

void RenderModule::AddShape(GLFWwindow* window, std::shared_ptr<ShapeEvent> event_ptr)
{
	std::unique_lock<std::mutex> locker(mRenderMtx);
	mRenderComplete = false;
	TakeControlOfContext(window);
	mShapeModule->AddEvent(event_ptr);
	while (!mRenderComplete)
	{
		mRenderCv.wait(locker);
	}
	ReleaseControlOfContext(window);
	
}


int CreateShaderProg(const char*, const char*);
int main(int argc, char* argv)
{
	AppController app;
	app.Initialize();
	GLFWwindow* window = app.GetWindowPtr();
	int shader_prog;
	try
	{
		shader_prog = CreateShaderProg(VERT_SHADER_PATH, FRAG_SHADER_PATH);
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
	WindowBackground window_back(0.2f, 0.3f, 0.3f, 1.0f);
	//std::shared_ptr<ShapeModule> shape_module = std::make_shared<ShapeModule>();
	
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	std::shared_ptr<RenderModule> render_module = std::make_shared<RenderModule>();
	render_module->mShapeModule->AddSubscriber(render_module);
	render_module->mShapeModule->Launch();
	render_module->Launch();
	std::shared_ptr<ShapeEvent> shape_event = std::make_shared<ShapeEvent>(EventName::SHAPE_ADDSHAPE, verts);
	shape_event->Set<GLuint>(EventName::SHAPE_ADDSHAPE_VAO, vao);
	shape_event->Set<GLuint>(EventName::SHAPE_ADDSHAPE_VBO, vbo);
	shape_event->Set<unsigned int>(EventName::SHAPE_ADDSHAPE_DATA_LENGTH, 9);
	render_module->AddShape(window, shape_event);
	//shape_module->HandleAddShapeEvent(shape_event);


	while (!glfwWindowShouldClose(window))
	{
		window_back.Draw();
		glUseProgram(shader_prog);
		render_module->Render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	render_module->mShapeModule->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
	render_module->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
	render_module->mShapeModule->join();
	render_module->join();
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