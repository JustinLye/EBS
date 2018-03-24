#include"graphics/window/WindowEvent.h"

//////////////////////////////////////////////////////////////////////
///\ fn WindowEvent::WindowEvent(const WindowEvent::name_t&, GLFWindow* window)
///\ brief Constructor
///\ author Justin Lye
///\ date 03/22/2018
//////////////////////////////////////////////////////////////////////

WindowEvent::WindowEvent(const WindowEvent::name_t& event_name, GLFWwindow* window) :
	BaseEvent(event_name),
	mWindow(window)
{

}



//////////////////////////////////////////////////////////////////////
///\ fn WindowEvent::GetWindowPtr()
///\ brief Get the context this event is related to 
///\ returns pointer to GLFWwindow
///\ author Justin Lye
///\ date 03/22/2018
//////////////////////////////////////////////////////////////////////

GLFWwindow* WindowEvent::GetWindowPtr()
{
	return mWindow;
}


std::shared_ptr<WindowEvent> WindowEvent::MakeWindowEventPtr(const WindowEvent::name_t& event_name, GLFWwindow* window)
{
	return std::make_shared<WindowEvent>(event_name, window);
}