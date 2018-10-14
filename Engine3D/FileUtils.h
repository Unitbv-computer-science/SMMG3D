#pragma once

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	bool ReadTextFile(const char* fileName, std::string& outFile);
	bool ReadBinaryFile(const char* pFileName, std::shared_ptr<char>& spData, size_t& dataSize);
};

