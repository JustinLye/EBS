#ifndef WINDOW_CONTROLLER_HEADER_INCLUDED
#define WINDOW_CONTROLLER_HEADER_INCLUDED

#include"graphics/WindowEventInterface.h"

class WindowController :
	private WindowEventModule
{
protected:
	std::shared_ptr<WindowEventModule> mModule;
	WindowEventInterface* mEventInterface;

	void HandleKeyPress(std::shared_ptr<WindowEvent>);
public:
	WindowController();
	virtual ~WindowController();
};

#endif