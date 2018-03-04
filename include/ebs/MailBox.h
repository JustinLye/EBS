#ifndef MAIL_BOX_HEADER_INCLUDED
#define MAIL_BOX_HEADER_INCLUDED

#include<queue>
#include<mutex>
#include<condition_variable>

template<typename T>
class MailBox : private std::queue<T>
{
	std::mutex mCheckMailMtx;
	std::condition_variable mMailReceivedEvent;
public:
	MailBox();
	T GetMail();
	void AcceptMail(T);
	bool Empty() const;
};

#ifndef MAIL_BOX_CC_INCLUDED
#include"ebs/MailBox.cc"
#endif

#endif