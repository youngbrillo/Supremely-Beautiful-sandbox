#define _CRT_SECURE_NO_WARNINGS
#include "SettingsFile.h"
#include "FileIOoperations.h"
#include "sajson.h"

FileIO::SettingsFile::SettingsFile(const char* FileName) 
	: FileData(FileName)
	, m_index (0)
	, m_windowWidth(1200)
	, m_windowHeight(500)
{
	if (FileIO::LoadFile(this->data, dataSize, FileName))
	{
		this->onLoad();
	}
}

void FileIO::SettingsFile::onSave(FILE* file)
{

	fprintf(file, "{\n");
	fprintf(file, "  \"testIndex\": %d,\n", m_index);
	fprintf(file, "  \"windowWidth\": %d,\n", m_windowWidth);
	fprintf(file, "  \"windowHeight\": %d\n", m_windowHeight);
	fprintf(file, "}\n");
}

void FileIO::SettingsFile::onLoad()
{
	const sajson::document& document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(dataSize, data));
	if (document.is_valid() == false)
	{
		int line = document.get_error_line();
		int column = document.get_error_column();
		const char* message = document.get_error_message_as_cstring();
		printf("Settings File (%s) is not valid\n\tLine %d, column %d:\t%s\n", this->fileName.c_str(), line, column, message);
		return;
	}

	sajson::value root = document.get_root();
	int fieldCount = int(root.get_length());
	for (int i = 0; i < fieldCount; ++i)
	{
		sajson::string fieldName = root.get_object_key(i);
		sajson::value fieldValue = root.get_object_value(i);

		if (strncmp(fieldName.data(), "testIndex", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_index = fieldValue.get_integer_value();
			}
			continue;
		}

		if (strncmp(fieldName.data(), "windowWidth", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_windowWidth = fieldValue.get_integer_value();
			}
			continue;
		}

		if (strncmp(fieldName.data(), "windowHeight", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_windowHeight = fieldValue.get_integer_value();
			}
			continue;
		}
	}


	printf("Successfully Loaded file: %s\n", fileName.c_str());
}
