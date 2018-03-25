#ifndef BASE_MODULE_CONTROLLER_CC_INCLUDED
#define BASE_MODULE_CONTROLLER_CC_INCLUDED
#include"ebs/BaseModuleController.h"

#ifdef BUILD_BASE_MODULE_CONTROLLER_CC
BaseModuleController::BaseModuleController()
{

}

BaseModuleController::~BaseModuleController()
{

}

unsigned int BaseModuleController::AddModule()
{
	std::shared_ptr<BaseModule> ptr = std::make_shared<BaseModule>();
	unsigned int id = ptr->GetId();
	mModuleMap.insert({ id, ptr });
	return id;
}

void BaseModuleController::ShutdownModule(const unsigned int& id)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair != mModuleMap.end())
	{
		if (module_pair->second->joinable())
		{
			module_pair->second->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
			if (module_pair->second->joinable())
			{
				module_pair->second->join();
			}
		}
	}
}

void BaseModuleController::ShutdownModules()
{
	for (auto module_pair : mModuleMap)
	{
		if (module_pair.second->joinable())
		{
			module_pair.second->AddEvent(std::make_shared<BaseEvent>(EventName::SHUTDOWNEVENT));
		}
	}
	for (auto module_pair : mModuleMap)
	{
		if (module_pair.second->joinable())
		{
			module_pair.second->join();
		}
	}
}

void BaseModuleController::LaunchModule(const unsigned int& id)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair != mModuleMap.end())
	{
		module_pair->second->Launch();
	}
}

unsigned int BaseModuleController::AddThenLaunchModule()
{
	unsigned int id = AddModule();
	LaunchModule(id);
	return id;
}


#endif

template<class ModuleType>
unsigned int BaseModuleController::AddModule()
{
	std::shared_ptr<BaseModule> ptr = std::make_shared<ModuleType>();
	unsigned int id = ptr->GetId();
	mModuleMap.insert({ id, ptr });
	return id;
}

template<class ModuleType>
unsigned int BaseModuleController::AddThenLaunchModule()
{
	unsigned int id = AddModule<ModuleType>();
	LaunchModule(id);
	return id;
}


#ifdef BUILD_BASE_MODULE_CONTROLLER_CC

void BaseModuleController::AddEvent(const unsigned int& id, std::shared_ptr<BaseEvent> event_ptr)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	module_pair->second->AddEvent(event_ptr);
}

#endif


template<class ModuleType, class HandlerType, class EventType>
void BaseModuleController::RegisterEventHandlerForModule(
	const unsigned int& id,
	const EventName::event_name_t& event_name,
	void(HandlerType::*funct_ptr)(std::shared_ptr<EventType>))
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	module_pair->second->RegisterEventHandler(event_name, funct_ptr);
}

template<class ModuleType>
void BaseModuleController::AddSubscriber(const unsigned int& id, std::shared_ptr<ModuleType> module_ptr)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	module_pair->second->AddSubscriber(module_ptr);
}

template<class ModuleType>
void BaseModuleController::SubscribeToModule(const unsigned int& id, std::shared_ptr<ModuleType> module_ptr)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	std::shared_ptr<BaseModule>(module_ptr)->AddSubscriber(module_pair->second);
}


#endif