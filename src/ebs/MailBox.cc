#ifndef MAIL_BOX_CC_INCLUDED
#define MAIL_BOX_CC_INCLUDED

#include"ebs/MailBox.h"

#ifdef BUILD_MAIL_BOX_CC
#endif

template<typename T>
inline bool MailBox<T>::Empty() const
{
	return empty();
}

template<typename T>
MailBox<T>::MailBox() :
	std::queue<T>()
{

}

template<typename T>
T MailBox<T>::GetMail()
{
	std::unique_lock<std::mutex> lockbox(mCheckMailMtx);
	while (empty())
	{
		mMailReceivedEvent.wait(lockbox);
	}
	T mail = front();
	pop();
	return mail;
}

template<typename T>
void MailBox<T>::AcceptMail(T mail)
{
	std::unique_lock<std::mutex> lockbox(mCheckMailMtx);
	try
	{
		push(mail);
	} catch (std::exception& error)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(error.what()));
	} catch (...)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE("Unknown error thrown"));
	}
	mMailReceivedEvent.notify_all();
}


#endif