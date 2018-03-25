#ifndef BASE_MODULE_CONTROLLER_HEADER_INCLUDED
#define BASE_MODULE_CONTROLLER_HEADER_INCLUDED
#include<map>
#include"ebs/BaseModule.h"

class BaseModuleController
{
protected:
	std::map<unsigned int, std::shared_ptr<BaseModule>> mModuleMap;
public:
	BaseModuleController();
	virtual ~BaseModuleController();

	// Module creation and destruction
	unsigned int AddModule();
	void ShutdownModule(const unsigned int&);
	void ShutdownModules();
	void LaunchModule(const unsigned int&);
	unsigned int AddThenLaunchModule();

	// Template module creation and desctruction
	template<class ModuleType>
	unsigned int AddModule();
	template<class ModuleType>
	void LaunchModule(const unsigned int&);
	template<class ModuleType>
	unsigned int AddThenLaunchModule();

	// Module interaction
	void AddEvent(std::shared_ptr<BaseEvent>);

	template<class ModuleType, class HandlerType = ModuleType, class EventType = BaseEvent>
	void RegisterEventHandlerForModule(
		const unsigned int&,
		const EventName::event_name_t&,
		void(HandlerType::*funct_ptr)(std::shared_ptr<EventType>));

	// Template module interaction
	template<class ModuleType>
	void AddSubscriber(const unsigned int&, std::shared_ptr<ModuleType>);
	template<class ModuleType>
	void SubscribeToModule(const unsigned int&, std::shared_ptr<ModuleType>);
};


#ifdef BUILD_BASE_MODULE_CONTROLLER_CC
#include"ebs/BaseModuleController.cc"
#endif

#endif