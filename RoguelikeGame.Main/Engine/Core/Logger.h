#pragma once

#include "../Helpers/StringHelper.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <map>

struct LogOptions
{
	enum class LogOutput { CONSOLE, FILE, BOTH };

	std::string loggingPattern;
	LogOutput outputStream = LogOutput::CONSOLE;

	std::string filePath;
	bool fileAppend = false;
	std::map<std::string, bool> ignoredTypes;
};

class Logger
{
protected:
	Logger(const LogOptions options) : _options(options)
	{
		_options = options;
		_typeStringMap[LogType::DEBUG] = "DEBUG";
		_typeStringMap[LogType::ERROR] = "ERROR";
		_typeStringMap[LogType::INFO] = "INFO";
		_typeStringMap[LogType::WARNING] = "WARNING";

		_typeColorMap[LogType::DEBUG] = "36"; //FG - cyan, BG - black
		_typeColorMap[LogType::ERROR] = "1;41;37"; //FG - white, BG - red
		_typeColorMap[LogType::INFO] = "92"; //FG - b. green, BG - black
		_typeColorMap[LogType::WARNING] = "33"; //FG - yellow, BG - black

		if (_options.outputStream == LogOptions::LogOutput::FILE || _options.outputStream == LogOptions::LogOutput::BOTH)
		{
			_outputStream.open(_options.filePath, (_options.fileAppend) ? std::ios::app : std::ios::out);
		}
	}
    static Logger* _logger;
    LogOptions _options;
	std::string _lastMessage;
public:
	enum class LogType{ DEBUG, INFO, WARNING, ERROR };
	
    Logger(Logger& other) = delete;
    void operator=(const Logger&) = delete;

    static Logger* GetInstance(const LogOptions& options);
	static Logger* GetInstance();
    LogOptions options() const {
        return _options;
    }
	~Logger();

	void Log(LogType type, std::string message, bool ignoreDuplicants = false);
	static std::string CurrentDate();
	static std::string CurrentTime();
private:
	std::ofstream _outputStream;
	std::map<LogType, std::string> _typeStringMap;
	std::map<LogType, std::string> _typeColorMap;

	std::string MessageBuilder(LogType type, std::string message, bool colorType);
};