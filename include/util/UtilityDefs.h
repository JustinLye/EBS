#ifndef UTILITY_DEFS_HEADER_INCLUDED
#define UTILITY_DEFS_HEADER_INCLUDED

#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<boost/lexical_cast.hpp>

#if !defined(IS_WINDOWS) && !defined(IS_LINUX)
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define IS_WINDOWS
#elif defined(linux) || defined(unix) || defined(_linux) || defined(__linux) || defined(__linux__) || defined(_UNIX)
#define IS_LINUX
#else
#error OS not defined
#endif
#endif

#ifndef GET_BASENAME
#include<string>
#ifdef IS_WINDOWS
#define GET_BASENAME ((strrchr(__FILE__, '\\')) ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define GET_BASENAME ((strrchr(__FILE__, '/')) ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#endif

#ifndef DRESS_MSG
#define DRESS_MSG(msg) std::string(std::string(GET_BASENAME) + ":" + std::string(__FUNCTION__) + "() line " + boost::lexical_cast<std::string>(__LINE__) + ":\t" + std::string(msg))
#endif

#if defined(MAKEDLL)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif //DLLEXPORT

//Get system error is a method to quickly retrieve a system error
#if !defined(GET_SYSTEM_ERROR)
#if defined(IS_WINDOWS)
#if !defined(FormatMessage)
#include<Windows.h>
#endif
#define GET_SYSTEM_ERROR(errorCode, pBuffer) \
                    FormatMessage( \
                        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, \
                        nullptr, \
                        errorCode, \
                        0, \
                        (LPSTR)&pBuffer, \
                        0, \
                        nullptr \
                    )
#elif defined(IS_LINUX)
#include<errno.h>
#include<string.h>
#define GET_SYSTEM_ERROR(Error_code, Buffer) \
                    strerror_r(Error_code, NULL, 0)
#else
#error OS not defined
#endif
#endif //GET_SYSTEM_ERROR

//FILE_TO_STR reads content of file into a string
#if !defined(FILE_TO_CSTR)
#include<string>
#include<fstream>
#include<sstream>
#include<stdexcept>
#define FILE_TO_CSTR(Filepath, File_content) \
    { \
        File_content.clear(); \
	std::ifstream File_stream; \
	std::stringstream File_string_stream; \
	File_stream.exceptions(std::ifstream::badbit); \
	try { \
		File_stream.open(Filepath); \
		File_string_stream << File_stream.rdbuf(); \
		File_stream.close(); \
		File_content = File_string_stream.str().c_str(); \
	} catch (std::ifstream::failure& e) { \
		File_content = ""; \
		throw std::runtime_error(e.what()); \
	} \
    }   

namespace util {

	struct Message_source {
		const char* File_name;
		const char* Function_name;
		int Line_number;
		int Thread_id;
		Message_source(
			const char* File = "NULL",
			const char* Function = "NULL",
			const int& Line = 0
		);
		friend std::ostream& operator<<(std::ostream& o, const Message_source& mb);
	};

	class Message {
	public:
		Message() {}
		Message(const Message_source& Msgb) : Msg_blck(Msgb) {}
		operator std::string();
		friend std::ostream& operator<<(std::ostream& o, const Message& m);
	protected:
		Message_source Msg_blck;

		virtual const char* type() const = 0;
		virtual const char* detail() const = 0;
		virtual void output(std::ostream& o) const;
	};

	class Info_message : public Message {
	public:
		Info_message() : Message(), Message_desc("NULL") {}
		Info_message(const util::Message_source& mb, const char* Desc = "NULL") :
			Message(mb),
			Message_desc(Desc) {}
	protected:
		const char* Message_desc;
		virtual const char* type() const;
		virtual const char* detail() const;
	};

	class System_error_message : public Message {
	public:
		System_error_message();
		System_error_message(const util::Message_source& mb, const int& Error_code);
	protected:
		int System_error_code;
		const char* System_error_desc;
		virtual const char* type() const;
		virtual const char* detail() const;
	};

	class Application_error_message : public Info_message {
	public:
		Application_error_message();
		Application_error_message(const util::Message_source& mb, const char* Desc = "NULL");
	protected:
		virtual const char* type() const;

	};

} //jl util

#define MSG_DIV "----------------------------------------------------"
#define MSG_DIV2 "*******************************************************************************************************"
#define MSG_NEWLOGGING_SESSION "NEW LOGGING SESSION STARTED"
#define MSG_CLOSELOGGIN_SESSION "CLOSING LOGGING SESSION"
#if !defined(MESSAGE_SOURCE)
#define MESSAGE_SOURCE util::Message_source(JL_FILENAME, __FUNCTION__, __LINE__)
#endif
#if !defined(INFO)
#define INFO(Message_desc) util::Info_message(MESSAGE_SOURCE, Message_desc)
#endif
#if !defined(INFO_MESSAGE)
#define INFO_MESSAGE(Message_desc) util::Info_message(MESSAGE_SOURCE, Message_desc)
#endif
#if !defined(SYS_ERROR_MESSAGE)
#define SYS_ERROR_MESSAGE(Sys_error_code) util::System_error_message(MESSAGE_SOURCE, Sys_error_code)
#endif

#if !defined(SYSERR)
#define SYSERR(Sys_error_code) util::System_error_message(MESSAGE_SOURCE, Sys_error_code)
#endif

#if !defined(USR_ERROR_MESSAGE)
#define APP_ERROR_MESSAGE(Message_desc) util::Application_error_message(MESSAGE_SOURCE, Message_desc)
#endif

#if !defined(APPERR)
#define APPERR(Message_desc) util::Application_error_message(MESSAGE_SOURCE, Message_desc)
#endif

#include"util/xml_tools.h"

#endif
