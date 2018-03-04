/////////////////////////////////////////////////////////
///\ file CallBack.cc
///\ brief Implements CallBack class
/////////////////////////////////////////////////////////

#ifndef CALLBACK_CC_INCLUDED
#define CALLBACK_CC_INCLUDED
#include"ebs/CallBack.h"


#ifdef BUILD_CALLBACK_CC
//This section is reserved for non-template definitions
const char* ERR_MSG_ATTEMPT_TO_RESET_CB = "Callback function has already been set.";
const char* ERR_MSG_CB_NOT_SET = "Callback function has not been set.";
#endif

template<typename R, class T, typename Arg>
CallBack<R, T, Arg>::CallBack() :
	mCallBackFunc(std::function<R(Arg)>())
{
}
template<typename R, class T, typename Arg>
CallBack<R, T, Arg>::CallBack(const CallBack& other) :
	mCallBackFunc(other.mCallBackFunc)
{
}
template<typename R, class T, typename Arg>
CallBack<R, T, Arg>::CallBack(std::function<R(Arg)> funct) :
	mCallBackFunc(funct)
{
}
template<typename R, class T, typename Arg>
CallBack<R, T, Arg>::CallBack(funct_ptr_t funct_ptr, T* ptr) :
	mCallBackFunc(std::bind(funct_ptr, ptr, std::placeholders::_1))
{
}
template<typename R, class T, typename Arg>
void CallBack<R, T, Arg>::SetCallBackFunction(funct_ptr_t funct_ptr, T* ptr)
{
	if (mCallBackFunc)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(ERR_MSG_ATTEMPT_TO_RESET_CB));
	}
	mCallBackFunc = std::bind(funct_ptr, ptr, std::placeholders::_1);
}
template<typename R, class T, typename Arg>
R CallBack<R, T, Arg>::operator()(Arg arg)
{
	if (!mCallBackFunc)
	{
		throw std::runtime_error(APP_ERROR_MESSAGE(ERR_MSG_CB_NOT_SET));
	}
	mCallBackFunc(arg);
}
template<typename R, class T, typename Arg>
CallBack<R, T, Arg>& CallBack<R, T, Arg>::operator=(const CallBack& other)
{
	mCallBackFunc = other.mCallBackFunc;
	return *this;
}


#endif