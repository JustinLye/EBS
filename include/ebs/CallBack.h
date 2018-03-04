#ifndef CALLBACK_HEADER_INCLUDED
#define CALLBACK_HEADER_INCLUDED
#include <stdexcept>
#include <functional>
#include "util/msgdefs.h"


//////////////////////////////////////////////
///\ class CallBack
///\ brief Provides container for member
///  function calls that take a single argument
//////////////////////////////////////////////
template<typename R, class T, typename Arg = void>
class CallBack
{
	typedef void (T::*funct_ptr_t)(Arg);
protected:
	std::function<R(Arg)> mCallBackFunc;
public:
	CallBack();
	CallBack(const CallBack&);
	CallBack(std::function<R(Arg)>);
	CallBack(funct_ptr_t, T*);
	
	void SetCallBackFunction(funct_ptr_t, T*);

	R operator()(Arg = 0);
	CallBack& operator=(const CallBack&);
};

#ifndef CALLBACK_CC_INCLUDED
#include"actor-model/CallBack.cc"
#endif
#endif