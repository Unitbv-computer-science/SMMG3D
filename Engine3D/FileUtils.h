#pragma once

#include <string>
#include <memory>

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	bool ReadTextFile(const char* fileName, std::string& outFile);
	bool ReadBinaryFile(const char* pFileName, std::shared_ptr<char>& spData, size_t& dataSize);

	static std::string getPath(const std::string& path);
	static std::string getFilename(const std::string& path);
	static std::string getFilenameExtensionless(const std::string& path);
	static std::string getExtension(const std::string& path);
};

