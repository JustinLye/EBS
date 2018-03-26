#ifndef CONTROLLER_HEADER_INCLUDED
#define CONTROLLER_HEADER_INCLUDED
#include<map>
#include"ebs/Module.h"

class Controller
{
protected:
	std::map<unsigned int, std::shared_ptr<Module>> mModuleMap;
public:
	Controller();
	virtual ~Controller();

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
	unsigned int AddThenLaunchModule();

	// Module interaction
	void AddEvent(const unsigned int&, std::shared_ptr<Event>);

	template<class ModuleType, class HandlerType = ModuleType, class EventType = Event>
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


#ifndef CONTROLLER_CC_INCLUDED
#include"ebs/Controller.cc"
#endif

#endif