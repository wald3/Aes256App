#pragma once
#include <string>
#include <vector>

typedef std::vector<unsigned char> ByteArray;

class FileAES
{
public:
	FileAES(std::string path);

	std::vector<unsigned char> ReadFile();

	void WriteToFile(ByteArray content, bool isEncrypted);

private:
	std::string Path;
};

