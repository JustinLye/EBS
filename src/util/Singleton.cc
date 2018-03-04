#include"util/Singleton.h"

std::shared_ptr<Singleton> Singleton::GetInstance()
{
	if (!mInstance)
	{
		mInstance = std::shared_ptr<Singleton>(new Singleton);
	}
	return mInstance;
}