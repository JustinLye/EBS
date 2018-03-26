#include"util/FileReader.h"

FileReader* FileReader::mInstance = nullptr;

FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

void FileReader::FileToString(const char* file_path, std::string& file_content)
{
	std::lock_guard<std::mutex> Locker(mReadMtx);
	file_content.clear();
	std::ifstream Input_file;
	std::stringstream File_str_stream;
	Input_file.exceptions(std::ifstream::badbit);
	Input_file.open(file_path);
	if (!Input_file)
	{
		throw std::runtime_error("Input file could not be opened");
	}
	File_str_stream << Input_file.rdbuf();
	Input_file.close();
	file_content = File_str_stream.str();
}

FileReader* FileReader::GetInstance()
{
	if (mInstance == nullptr)
		mInstance = new FileReader;
	return mInstance;
}