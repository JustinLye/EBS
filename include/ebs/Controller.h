
#include"ebs/BaseModule.h"

template<class EventType>
class Controller :
	public BaseModule<EventType>
{
protected:
	static Controller<EventType>* mInstance;
	std::shared_ptr<BaseModule<EventType>> mModule;
	Controller() :
		BaseModule<EventType>(1)
	{
		mModule = std::make_shared<BaseModule<EventType>>(1);
	}
	static Controller<EventType>* GetInstance()
	{
		if (!mInstance)
		{
			mInstance = new Controller<EventType>();
			mInstance->AddSubscriber(mInstance->mModule);
		}
		return mInstance;
	}
};

template<class EventType>
Controller<EventType>* Controller<EventType>::mInstance = nullptr;
