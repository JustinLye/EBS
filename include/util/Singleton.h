#ifndef SINGLETON_HEADER_INCLUDED
#define SINGLETON_HEADER_INCLUDED
#include<memory>
class Singleton
{
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton(Singleton&&) {}
	Singleton& operator=(const Singleton&) {}
	static std::shared_ptr<Singleton> mInstance;
public:
	static std::shared_ptr<Singleton> GetInstance();
	virtual ~Singleton() {}
};

#endif