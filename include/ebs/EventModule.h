#ifndef EVENT_HANDLER_HEADER_INCLUDED
#define EVENT_HANDLER_HEADER_INCLUDED

#include"util/UtilityDefs.h"
#include"util/Thread.h"
#include"ebs/MailBox.h"
#include"ebs/CallBack.h"
#include"ebs/Event.h"

class EventModule;
typedef std::shared_ptr<CallBack<void, EventModule, event_ptr>> event_handler_ptr;
typedef std::pair<event_ptr, event_handler_ptr> bound_event_handler;
typedef unsigned int module_id_t;
typedef std::map<EventName::event_name_t, event_handler_ptr> event_handler_map;

class EventModule :
	public Thread
{
	typedef EventName::event_name_t name_t;
	const module_id_t mId;
	event_handler_map mEventHandlerMap;
	MailBox<bound_event_handler> mMailBox;
	std::vector<std::weak_ptr<EventModule>> mSubscribers;
	EventModule() :
		mId(0),
		Thread()
	{
	}
protected:
	bool mShutdown;
	void EntryPoint();
	virtual void EventLoop();
	virtual void HandleShutdown(event_ptr);
	virtual event_handler_ptr LookupEventHandler(event_ptr);
public:
	explicit EventModule(const module_id_t&);
	virtual ~EventModule();
	template<typename T>
	void RegisterEventHandler(name_t, void(T::*funct_ptr)(event_ptr));
	void AddEvent(event_ptr);
	void AddSubscriber(std::shared_ptr<EventModule>);
	void SendToSubscribers(event_ptr);

};

#ifdef EVENT_MODULE_CC_INCLUDED
#include"ebs/EventModule.cc"
#endif
#endif