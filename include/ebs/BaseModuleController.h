#ifndef BASE_MODULE_CONTROLLER_HEADER_INCLUDED
#define BASE_MODULE_CONTROLLER_HEADER_INCLUDED

#include"ebs/BaseModule.h"

class BaseModuleController
{
protected:
	std::shared_ptr<BaseModule> mModule;
	
public:
	BaseModuleController();
	virtual ~BaseModuleController();
	virtual void ShutdownModule();
	virtual void LaunchModule();
	template<class ModuleType>
	void LaunchModule();
};

void BaseModuleController::ShutdownModule()
{
	if (mModule->joinable())
	{
		mModule->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
		if (mModule->joinable())
		{
			mModule->join();
		}
	}
}

BaseModuleController::BaseModuleController() :
	mModule(std::make_shared<BaseModule>())
{
	
}

BaseModuleController::~BaseModuleController()
{
}

void BaseModuleController::LaunchModule()
{
	mModule->Launch();
}

template<class ModuleType>
void BaseModuleController::LaunchModule()
{
	std::shared_ptr<BaseModule> ptr = std::make_shared<ModuleType>();
	mModule = std::shared_ptr<BaseModule>(ptr);
	mModule->Launch();
}


#endif