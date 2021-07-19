#pragma once

#include "../Helpers/StringHelper.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <array>
#include <map>

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008

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
	Logger(const LogOptions& options) : _options(options)
	{
		TurnOnColorsSupport();

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

	void TurnOnColorsSupport();
	std::string MessageBuilder(const LogType& type, const std::string& message, bool colorType);
};

class Stopwatch
{
protected:
	static Stopwatch* _instance;
	Stopwatch() { ; }
private:
	std::map<std::string, std::chrono::high_resolution_clock::time_point> _stopwatches;
public:
	static Stopwatch* GetInstance();

	Stopwatch(Stopwatch& other) = delete;
	void operator=(const Stopwatch&) = delete;

	void Start(const std::string& name);
	std::chrono::microseconds Stop(const std::string& name);
};