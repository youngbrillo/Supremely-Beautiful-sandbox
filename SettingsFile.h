#pragma once
#include "FileData.h"
namespace FileIO {

class SettingsFile : public FileData
{
public:
	SettingsFile(const char* FileName);
	~SettingsFile() {}

	void onSave(FILE* file);
	void onLoad() override;

public:
	int m_index;
	int m_windowWidth;
	int m_windowHeight;
};

}

