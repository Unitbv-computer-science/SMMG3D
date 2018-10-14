#include "stdafx.h"
#include "FileUtils.h"

#include <fstream>

FileUtils::FileUtils()
{}


FileUtils::~FileUtils()
{}

bool FileUtils::ReadTextFile(const char* pFileName, std::string& outFile)
{
	std::ifstream fileStream(pFileName);
	if (!fileStream.is_open()) {
		return false;
	}

	std::string line;
	while (std::getline(fileStream, line)) {
		outFile.append(line);
		outFile.append("\n");
	}
	fileStream.close();

	return true;
}

bool FileUtils::ReadBinaryFile(const char* pFileName, std::shared_ptr<char>& spData, size_t& dataSize)
{
	std::ifstream fileStream(pFileName);
	if (!fileStream.is_open()) {
		dataSize = 0;
		return false;
	}

	dataSize = fileStream.gcount();
	char *pData = new char[dataSize];

	fileStream.seekg(0, std::ios::beg);
	fileStream.read(pData, dataSize);
	fileStream.close();

	spData.reset(pData);
	return true;
}