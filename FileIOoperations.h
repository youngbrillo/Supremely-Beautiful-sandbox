#pragma once
#include <string>
#include <stdlib.h>
#include <stdio.h>

namespace FileIO
{
	class FileData; //forward-referenced
	bool LoadFile(char*& data, int& size, const char* fileName);
	bool SaveFile(const char* fileName, FileData* data);
}
