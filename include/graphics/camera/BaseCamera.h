#ifndef BASE_CAMERA_HEADER_INCLUDED
#define BASE_CAMERA_HEADER_INCLUDED

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"graphics/window/WindowEventInterface.h"


class CameraModule :
	public Module
{
protected:
public:
	CameraModule();
	virtual ~CameraModule();

	virtual void HandleKeyPress(std::shared_ptr<WindowEvent>);

};

#endif