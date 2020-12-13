#pragma once

#include "StringHelper.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <map>

struct LogOptions
{
	enum class LogOutput { CONSOLE, FILE, BOTH };

	std::string loggingPattern;
	LogOutput outputStream;

	std::string filePath;
	bool fileAppend;
	std::map<std::string, bool> ignoredTypes;
};

class Logger
{
public:
	static enum class LogType{ DEBUG, INFO, WARNING, ERROR };
	
	Logger(LogOptions options);
	~Logger();

	void Log(LogType type, std::string message);
	static std::string CurrentDate();
	static std::string CurrentTime();
private:
	LogOptions _options;
	std::ofstream _outputStream;
	std::map<LogType, std::string> _typeStringMap;
	std::map<LogType, std::string> _typeColorMap;

	std::string MessageBuilder(LogType type, std::string message, bool colorType);
};


