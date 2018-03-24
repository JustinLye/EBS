#ifndef WINDOW_CONTROLLER_HEADER_INCLUDED
#define WINDOW_CONTROLLER_HEADER_INCLUDED

#include"ebs/BaseModule.h"
#include"graphics/window/WindowEventInterface.h"



class WindowController :
	private BaseModule /* Derived from BaseModule so that &WindowController::* can be registered as callback */
{
public:
	using BaseModule::GetId;
protected:
	std::shared_ptr<BaseModule> mModule;
	WindowEventInterface* mEventInterface;

	void HandleKeyPress(std::shared_ptr<WindowEvent>);
public:
	WindowController();
	virtual ~WindowController();
};

#endif