#include"graphics/camera/BaseCamera.h"

CameraModule::CameraModule() :
	Module()
{
	RegisterEventHandler(EventName::KEY_PRESS, &CameraModule::HandleKeyPress);
	
}

CameraModule::~CameraModule()
{

}

void CameraModule::HandleKeyPress(std::shared_ptr<WindowEvent> event_ptr)
{

}