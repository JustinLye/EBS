#ifndef SINGLETON_HEADER_INCLUDED
#define SINGLETON_HEADER_INCLUDED
#include<memory>
class Singleton
{
protected:
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton(Singleton&&) {}
	Singleton& operator=(const Singleton&) {}
	static std::shared_ptr<Singleton> mInstance;
	virtual void Initialize() {}
public:
	static std::shared_ptr<Singleton> GetInstance();
	virtual ~Singleton() {}
};

#endif