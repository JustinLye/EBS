#ifndef BASE_MODULE_HEADER_INCLUDED
#define BASE_MODULE_HEADER_INCLUDED

#include"util/UtilityDefs.h"
#include"util/Thread.h"
#include"ebs/MailBox.h"
#include"ebs/CallBack.h"

template<class EventType>
class BaseModule :
	public Thread
{
public:
	using event_ptr = std::shared_ptr<EventType>;
	using cb_ptr = std::shared_ptr<CallBack<void, BaseModule<EventType>, event_ptr>>;
	using bound_cb = std::pair<event_ptr, cb_ptr>;
	using cb_map = std::map<typename EventType::name_type, cb_ptr>;
	using event_type = unsigned int;

protected:
	bool mShutdown;
	void EntryPoint();
	bound_cb GetMail();
	virtual void EventLoop();
	virtual void HandleShutdown(event_ptr);
	virtual cb_ptr LookupEventHandler(event_ptr);
	const unsigned int mId;
	cb_map mEventHandlerMap;
	MailBox<bound_cb> mMailBox;
	std::vector<std::weak_ptr<BaseModule>> mSubscribers;
	BaseModule() :
		mId(0),
		Thread()
	{
	}
public:
	explicit BaseModule(const unsigned int&);
	virtual ~BaseModule();
	template<typename T>
	void RegisterEventHandler(typename EventType::name_type event_name, void(T::*funct_ptr)(std::shared_ptr<EventType>))
	{
		auto handler = mEventHandlerMap.find(event_name);
		if (handler != mEventHandlerMap.end() && event_name != EventName::SHUTDOWNEVENT)
		{
			throw std::runtime_error(APP_ERROR_MESSAGE("Attempted to register for the same event twice"));
		}
		
		mEventHandlerMap[event_name] = std::make_shared<CallBack<void, BaseModule<EventType>, std::shared_ptr<EventType>>>(reinterpret_cast<void(BaseModule<EventType>::*)(std::shared_ptr<EventType>)>(funct_ptr), this);
	}
	virtual void AddEvent(event_ptr);
	virtual void AddSubscriber(std::shared_ptr<BaseModule>);
	virtual void SendToSubscribers(event_ptr);
	static event_ptr MakeEventPtr(const typename EventType::name_type& event_name)
	{
		return std::make_shared<EventType>(event_name);
	}
};



#ifndef BASE_MODULE_CC_INCLUDED
#include"ebs/BaseModule.cc"
#endif
#endif