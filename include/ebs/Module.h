#ifndef MODULE_HEADER_INCLUDED
#define MODULE_HEADER_INCLUDED

#include"util/UtilityDefs.h"
#include"util/Thread.h"
#include"ebs/MailBox.h"
#include"ebs/CallBack.h"
#include"ebs/Event.h"


////////////////////////////////////////////////////////////////
///\ class Module
///\ brief Event processing mechanism.
///\ author Justin Lye
///\ date 03/21/2018
////////////////////////////////////////////////////////////////

enum module_state_t : unsigned int
{
	MODULE_NOT_READY,
	MODULE_IS_READY
};

class Module :
	public Thread
{
public:
	// Name aliases 
	using event_ptr = std::shared_ptr<Event>;
	using cb = CallBack<void, Module, event_ptr>;
	using cb_ptr = std::shared_ptr<cb>;
	using bound_cb = std::pair<event_ptr, cb_ptr>;
	using name_t = Event::name_t;
	using cb_map = std::map<name_t, cb_ptr>;
	//using event_type = unsigned int;
private:
	static unsigned int NextModuleId; ///< Used to automatically assign the module and id. Note, this does not mean the derived modules have to use this Id. Its mainly for convience
protected:
	bool mShutdown; ///< Signals it is time to shutdown the moduel. Every module needs to be able to shut itself down.
	module_state_t mModuleState; ///< Set to read when thread has entered event loop
	std::mutex mUpdateStateMtx; ///< Protect access while updating/querying module state
	std::condition_variable mUpdateStateCond; ///< Condition variable to wait on state 
	void EntryPoint(); ///< Implement pure virtual function Thread::EntryPoint()
	bound_cb GetMail(); ///< Receives next message from mailbox. Blocks until message is received (i.e. every module must be able to receive and handle the SHUTDOWN event).
	virtual void EventLoop(); ///< 
	virtual void HandleShutdown(event_ptr); ///< (currently) Default shutdown method that is registered during base claass construction. This method just sets mShutdown equal to true.
	virtual cb_ptr LookupEventHandler(event_ptr); ///< Method looks up the appropriate handler for the given event name.
	unsigned int mId; ///< Default Id assigned to the module
	cb_map mEventHandlerMap; ///< Maps event names to their handlers
	MailBox<bound_cb> mMailBox; ///< Queue used to receive messages. 
	std::vector<std::weak_ptr<Module>> mSubscribers; ///< All other modules that receive notifications from this module.
public:
	Module(); ///< Default constructor
	virtual ~Module(); ///< Destructor

	virtual void AddEvent(event_ptr);
	virtual void AddSubscriber(std::shared_ptr<Module>);
	virtual void SendToSubscribers(event_ptr);
	const unsigned int& GetId() const;
	bool IsShuttingDown() const;

	template<class T>
	void RegisterEventHandler(const name_t&, void(T::*funct_ptr)(event_ptr));
	template<class T, class E>
	void RegisterEventHandler(const name_t&, void(T::*funct_ptr)(std::shared_ptr<E>));

	static event_ptr MakeEventPtr(const name_t&);
};



#ifndef MODULE_CC_INCLUDED
#include"ebs/Module.cc"
#endif
#endif