#ifndef WINDOW_CONTROLLER_HEADER_INCLUDED
#define WINDOW_CONTROLLER_HEADER_INCLUDED

#include"ebs/Module.h"
#include"graphics/window/WindowEventInterface.h"



class WindowController :
	private Module /* Derived from BaseModule so that &WindowController::* can be registered as callback */
{
public:
	using Module::GetId;
protected:
	std::shared_ptr<Module> mModule;
	WindowEventInterface* mEventInterface;

	void HandleKeyPress(std::shared_ptr<WindowEvent>);
public:
	WindowController();
	virtual ~WindowController();
};

#endif