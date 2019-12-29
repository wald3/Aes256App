#include "FileAES.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <QFile>
#include <Qt>
#include <qdebug.h>

using namespace std;

FileAES::FileAES(std::string path)
{
	this->Path = path;
}

void remove_r(vector<unsigned char>& data)
{
	std::string toErase("\r\n");
	std::string str(data.begin(), data.end());

	size_t pos = std::string::npos;

	while ((pos = str.find(toErase)) != std::string::npos)
	{
		str.erase(pos, toErase.length());
		str.insert(pos, "\n");
	}
	data = vector<unsigned char>(str.begin(), str.end());
}

std::vector<unsigned char> FileAES::ReadFile()
{
	std::ifstream file(this->Path, std::ios::binary | std::ios::ate);

	file.unsetf(std::ios::skipws);

	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),
		std::istream_iterator<unsigned char>(file),
		std::istream_iterator<unsigned char>());

	remove_r(vec);
	return vec;
}

void FileAES::WriteToFile(std::vector<unsigned char> content, bool isEncrypted)
{
	std::string aes(".aes");
	std::ofstream outfile;
	if (isEncrypted)
		outfile.open(this->Path + aes);
	else
	{
		size_t pos = this->Path.find(aes);

		if (pos != std::string::npos)
		{
			outfile.open(this->Path.erase(pos, aes.length()));
		}
		else
			outfile.open(this->Path);
	}
		

	outfile.write((char*)&content.data()[0], content.size());
	outfile.close();
}