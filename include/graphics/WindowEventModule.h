#ifndef WINDOW_EVENT_MODULE_HEADER_INCLUDED
#define WINDOW_EVENT_MODULE_HEADER_INCLUDED

#include"ebs/BaseModule.h"
#include"graphics/WindowEvent.h"

class WindowEventModule : public BaseModule<WindowEvent>
{
public:
	WindowEventModule();
	virtual ~WindowEventModule();
};

#endif