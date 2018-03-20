#ifndef __FILE_READER_H_INCLUDED__
#define __FILE_READER_H_INCLDUED__

#include<string>
#include<fstream>
#include<sstream>
#include<mutex>

class FileReader
{
public:
	void FileToString(const char* File_path, std::string& File_content);
	static FileReader* GetInstance();
private:
	FileReader();
	FileReader(const FileReader&) {}
	FileReader(FileReader&&) {}
	~FileReader();
	static FileReader* mInstance;
	std::mutex mReadMtx;
};

#endif