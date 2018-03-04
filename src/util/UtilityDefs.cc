#include"util/UtilityDefs.h"
util::Message_source::Message_source(
	const char* File,
	const char* Function,
	const int& Line)
	:
	File_name(File),
	Function_name(Function),
	Line_number(Line),
	Thread_id(0) {}


void util::Message::output(std::ostream& o) const {
	o << "Type:\t" << type() << std::endl << Msg_blck << "Desc:\t" << detail();
}

std::ostream& util::operator<<(std::ostream& o, const util::Message_source& mb)
{
	o << "File:\t" << mb.File_name << "[" << mb.Line_number << "]" << std::endl;
	o << "Thread:\t" << mb.Thread_id << std::endl;
	o << "Method:\t" << mb.Function_name << std::endl;
	return o;
}

std::ostream& util::operator<<(std::ostream& o, const util::Message& m)
{
	m.output(o);
	return o;
}

const char* util::Info_message::type() const
{
	return "INFO";
}

const char* util::Info_message::detail() const
{
	return Message_desc;
}

util::System_error_message::System_error_message() :
	Message(),
	System_error_code(1),
	System_error_desc(nullptr)
{
	GET_SYSTEM_ERROR(System_error_code, System_error_desc);
}

util::System_error_message::System_error_message(
	const util::Message_source& mb,
	const int& Error_code) :
	Message(mb),
	System_error_code(Error_code),
	System_error_desc(NULL)
{
	GET_SYSTEM_ERROR(System_error_code, System_error_desc);
}

util::Message::operator std::string()
{
	std::stringstream ss;
	ss << type() << std::endl;
	ss << Msg_blck << std::endl;
	ss << detail() << std::endl;
	std::string temp = ss.str().c_str();
	return temp.c_str();
}


const char* util::System_error_message::type() const
{
	return "SYSTEM ERROR";
}

const char* util::System_error_message::detail() const
{
	return System_error_desc;
}


const char* util::Application_error_message::type() const
{
	return "APPLICATION ERROR";
}

util::Application_error_message::Application_error_message() :
	Info_message() {}

util::Application_error_message::Application_error_message(
	const util::Message_source& mb,
	const char* Desc) :
	Info_message(mb, Desc) {}