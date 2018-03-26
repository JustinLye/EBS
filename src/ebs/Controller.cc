#ifndef CONTROLLER_CC_INCLUDED
#define CONTROLLER_CC_INCLUDED
#include"ebs/Controller.h"

#ifdef BUILD_CONTROLLER_CC
Controller::Controller()
{

}

Controller::~Controller()
{

}

unsigned int Controller::AddModule()
{
	std::shared_ptr<Module> ptr = std::make_shared<Module>();
	unsigned int id = ptr->GetId();
	mModuleMap.insert({ id, ptr });
	return id;
}

void Controller::ShutdownModule(const unsigned int& id)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair != mModuleMap.end())
	{
		if (module_pair->second->joinable())
		{
			module_pair->second->AddEvent(std::make_shared<Event>(EventName::SHUTDOWNEVENT));
			if (module_pair->second->joinable())
			{
				module_pair->second->join();
			}
		}
	}
}

void Controller::ShutdownModules()
{
	for (auto module_pair : mModuleMap)
	{
		if (module_pair.second->joinable())
		{
			module_pair.second->AddEvent(std::make_shared<Event>(EventName::SHUTDOWNEVENT));
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

void Controller::LaunchModule(const unsigned int& id)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair != mModuleMap.end())
	{
		module_pair->second->Launch();
	}
}

unsigned int Controller::AddThenLaunchModule()
{
	unsigned int id = AddModule();
	LaunchModule(id);
	return id;
}


#endif

template<class ModuleType>
unsigned int Controller::AddModule()
{
	std::shared_ptr<Module> ptr = std::make_shared<ModuleType>();
	unsigned int id = ptr->GetId();
	mModuleMap.insert({ id, ptr });
	return id;
}

template<class ModuleType>
unsigned int Controller::AddThenLaunchModule()
{
	unsigned int id = AddModule<ModuleType>();
	LaunchModule(id);
	return id;
}


#ifdef BUILD_CONTROLLER_CC

void Controller::AddEvent(const unsigned int& id, std::shared_ptr<Event> event_ptr)
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
void Controller::RegisterEventHandlerForModule(
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
void Controller::AddSubscriber(const unsigned int& id, std::shared_ptr<ModuleType> module_ptr)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	module_pair->second->AddSubscriber(module_ptr);
}

template<class ModuleType>
void Controller::SubscribeToModule(const unsigned int& id, std::shared_ptr<ModuleType> module_ptr)
{
	auto module_pair = mModuleMap.find(id);
	if (module_pair == mModuleMap.end())
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Module id is not mapped to this controller."));
	}
	std::shared_ptr<Module>(module_ptr)->AddSubscriber(module_pair->second);
}


#endif