#pragma once
#include "../../Msvc/FrameworkStd.h"


/*

This file defines a logger-struct and writes log-information into LogFile.txt
the Output-Path of this file is usually located at the output directory of the assembly

*/

#if WITH_LOGGING == 1
#include <cstdio>
#include <string>
#include <fstream>
#include <ios>
#define LOG(str) {if(str){ LogFileWriter(str); }}

const static char* FilePath = "LogFile.txt";
static bool IsClean = false;

struct LogFileWriter
{

public:

	LogFileWriter()
	{
		WriteToLog("No loggable parameter given");
	}

	LogFileWriter(int32 Value)
	{
		std::string tmpString = std::to_string(Value);

		if (&tmpString)
		{
			WriteToLog(tmpString);
		}
	}

	LogFileWriter(const char* logString)
	{
		if (logString != nullptr)
		{
			WriteToLog(logString);
		}
	}

	LogFileWriter(const std::string &logString)
	{
		if (&logString != nullptr)
		{
			WriteToLog(logString);
		}
	}

	~LogFileWriter()
	{
	}

private:
	void WriteToLog(const std::string &logString)
	{
		std::ofstream file(FilePath, std::ios_base::app | std::ios_base::out);

		if (!IsClean)
		{
			file.clear();
			IsClean = true;
		}

		file << logString << std::endl;		
	}
};
#endif




