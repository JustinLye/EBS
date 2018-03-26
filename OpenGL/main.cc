#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


//#include"util/Singleton.h"
//#include"graphics/WindowController.h"
#include"graphics/AppController.h"
//#include"util/FileReader.h"
#include"graphics/shape/BaseShape.h"
#include"graphics/shape/ShapeEvent.h"
#include"graphics/window/WindowBackground.h"
#include"graphics/shape/ShapeModule.h"
#include"graphics/shader/BaseShader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb/stb_image.h"
const char* VERT_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.vert";
const char* FRAG_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core.frag";
const char* FRAG2_SHADER_PATH = "C:\\EBS\\OpenGL\\shaders\\core2.frag";
const char* GORILLA_JPG_PATH = "C:\\EBS\\OpenGL\\textures\\gorilla.jpg";
const char* OSCAR_JPG_PATH = "C:\\EBS\\OpenGL\\textures\\oscar.jpg";


class RenderModule :
	public Module
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
	void HandleRenderAckEvent(std::shared_ptr<Event>);
	void HandleContextAckEvent(std::shared_ptr<Event>);
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
	Module(),
	mRenderComplete(false),
	mContextSwitchComplete(false),
	mShapeModule(std::make_shared<ShapeModule>())
{
	RegisterEventHandler(EventName::RENDER_DRAWING_COMPLETE, &RenderModule::HandleRenderAckEvent);
	RegisterEventHandler(EventName::SHAPE_ADDSHAPE_COMPLETE, &RenderModule::HandleRenderAckEvent);
	RegisterEventHandler(EventName::RENDER_CONTEXT_CHANGE_COMPLETE, &RenderModule::HandleContextAckEvent);
}

void RenderModule::HandleRenderAckEvent(std::shared_ptr<Event> event_ptr)
{
	std::unique_lock<std::mutex> locker(mRenderMtx);
	mRenderComplete = true;
	mRenderCv.notify_all();
}

void RenderModule::HandleContextAckEvent(std::shared_ptr<Event> event_ptr)
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
	std::shared_ptr<Event> draw_event_ptr = std::make_shared<Event>(EventName::RENDER_DRAW);
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


int main(int argc, char* argv)
{
	AppController app;
	app.Initialize();
	GLFWwindow* window = app.GetWindowPtr();
	std::shared_ptr<BaseShader> shader;
	try
	{
		shader = std::make_shared<BaseShader>(VERT_SHADER_PATH, FRAG_SHADER_PATH);
	}
	catch (std::exception& error)
	{
		std::cout << error.what() << '\n';
	}
	float verts0[] =
	{
		-0.9f, -0.9f, 0.0f,
		-0.4f, -0.9f, 0.0f,
		-0.65f, -0.2f, 0.0f,
	};
	float verts[] =
	{
		-0.9f, -0.9f, 0.0f,
		-0.4f, -0.9f, 0.0f,
		-0.65f, -0.2f, 0.0f,
		0.9f, 0.2f, 0.0f,
		0.4f, 0.2f, 0.0f,
		0.65f, 0.9f, 0.0f
	};
	float verts2[] =
	{
		0.9f, 0.2f, 0.0f,
		0.4f, 0.2f, 0.0f,
		0.65f, 0.9f, 0.0f
	};
	float verts3[] =
	{
		-0.25f, 0.0f, 0.0f,
		0.25f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	float cube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	unsigned int texture, oscar_texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(GORILLA_JPG_PATH, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glGenTextures(1, &oscar_texture);
	glBindTexture(GL_TEXTURE_2D, oscar_texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	//stbi_set_flip_vertically_on_load(true);
	data = stbi_load("C:\\EBS\\OpenGL\\textures\\oscar2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glUniform1i(glGetUniformLocation(shader->GetProgramId(), "texture1"), 0);
	WindowBackground window_back(0.2f, 0.3f, 0.3f, 1.0f);
	//std::shared_ptr<ShapeModule> shape_module = std::make_shared<ShapeModule>();
	
	/*GLuint vao, vbo;
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
	render_module->AddShape(window, shape_event);*/
	//shape_module->HandleAddShapeEvent(shape_event);

	GLuint vao;
	GLuint vbo[2];
	GLsizei size = 3 * sizeof(float);
	GLsizei strides[2] = { size, size };
	GLint offsets[2] = { 0,size };

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(2, vbo);
	glNamedBufferData(vbo[0], sizeof(cube), cube, GL_DYNAMIC_DRAW);
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);	
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo[0], 0, 5 * sizeof(float));
	//glBindVertexArray(0);

	/*glCreateVertexArrays(1, &vao);
	glCreateBuffers(2, vbo);
	glBindVertexArray(vao);
	glNamedBufferData(vbo[0], sizeof(verts0), verts0, GL_DYNAMIC_DRAW);
	glNamedBufferData(vbo[1], sizeof(verts2), verts2, GL_DYNAMIC_DRAW);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glVertexAttribBinding(1, 1);
	glBindVertexBuffer(0, vbo[0], 0, 3*sizeof(float));
	glBindVertexBuffer(1, vbo[1], 0, 3*sizeof(float));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);*/

	/*glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

	
	auto start_time = std::chrono::high_resolution_clock::now();
	auto start_time2 = std::chrono::high_resolution_clock::now();
	auto start_time3 = start_time2;
	bool use_verts3 = true;
	bool switch_arrays = false;
	unsigned int model_mat_loc = glGetUniformLocation(shader->GetProgramId(), "model_mat");
	unsigned int view_mat_loc = glGetUniformLocation(shader->GetProgramId(), "view_mat");
	unsigned int projection_mat_loc = glGetUniformLocation(shader->GetProgramId(), "projection_mat");
	glm::mat4 identity_mat = glm::mat4(1.0f);
	glm::mat4 model_mat = glm::mat4(1.0f);
	glm::mat4 view_mat = glm::mat4(1.0f);
	glm::mat4 projection_mat = glm::mat4(1.0f);
	int sh, sw;
	glfwGetFramebufferSize(app.GetWindowPtr(), &sw, &sh);
	view_mat = glm::translate(view_mat, glm::vec3(0.0f, 0.0f, -3.0f));
	projection_mat = glm::perspective(glm::radians(45.0f), (float)sw / (float)sh, 0.1f, 100.0f);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, glm::value_ptr(view_mat));
	glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_mat));
	unsigned int active_texture = texture;
	while (!glfwWindowShouldClose(window))
	{
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() > 3)
		{
			if (use_verts3)
			{
				//glNamedBufferSubData(vbo[0], 0, sizeof(verts3), verts3);
				
			}
			else
			{
				//glNamedBufferSubData(vbo[0], 0, sizeof(verts0), verts0);
			}
			use_verts3 = !use_verts3;
			active_texture = texture;
			start_time = std::chrono::high_resolution_clock::now();
			
		}
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time2).count() > 5)
		{
			switch_arrays = !switch_arrays;
			active_texture = oscar_texture;
			start_time2 = std::chrono::high_resolution_clock::now();
		}
		window_back.Draw();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, active_texture);
		glUseProgram(shader->GetProgramId());
		glBindVertexArray(vao);
		//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(identity_mat));
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time2).count() > 1)
		{
			model_mat = identity_mat;
			//model_mat = glm::rotate(model_mat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		} else
		{
			model_mat = glm::translate(model_mat, glm::vec3(-0.001f * (float)glfwGetTime()));
			model_mat = glm::rotate(model_mat, glm::radians((float)glfwGetTime()), glm::vec3(1.0f, 0.5f, 0.0f));
		}
		glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//render_module->mShapeModule->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
	//render_module->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
	//render_module->mShapeModule->join();
	//render_module->join();
	app.Shutdown();
	
	return EXIT_SUCCESS;
}

