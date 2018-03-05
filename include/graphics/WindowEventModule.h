#ifndef WINDOW_EVENT_MODULE_HEADER_INCLUDED
#define WINDOW_EVENT_MODULE_HEADER_INCLUDED

#include"ebs/BaseModule.h"
#include"graphics/WindowEvent.h"

class WindowEventModule : public BaseModule<WindowEvent>
{
public:
	explicit WindowEventModule(const unsigned int&);
	virtual ~WindowEventModule();
};

WindowEventModule::WindowEventModule(const unsigned int& id) :
	BaseModule<WindowEvent>(id)
{
}

WindowEventModule::~WindowEventModule()
{

}
#endif