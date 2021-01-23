#include "Logger.h"

#define NOGDI //To avoid Windows.h ERROR enum conflict
#include <Windows.h>

Logger* Logger::_logger = nullptr;

Logger* Logger::GetInstance(const LogOptions& options)
{
	if (_logger == nullptr) {
		_logger = new Logger(options);
	}
	return _logger;
}

Logger* Logger::GetInstance()
{
	return _logger;
}

Logger::~Logger()
{
	if (_options.outputStream == LogOptions::LogOutput::FILE || _options.outputStream == LogOptions::LogOutput::BOTH)
	{
		_outputStream.close();
	}
}

void Logger::TurnOnColorsSupport()
{
	DWORD l_mode;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hStdout, &l_mode);
	SetConsoleMode(hStdout, l_mode |
		ENABLE_VIRTUAL_TERMINAL_PROCESSING |
		DISABLE_NEWLINE_AUTO_RETURN);
}

std::string Logger::MessageBuilder(LogType type, std::string message, bool colorType)
{
	std::string pattern = _options.loggingPattern;
	
	if (colorType)
		pattern = StringHelper::ReplaceAll(pattern, "{type}", "\033[" + _typeColorMap[type] + "m" + _typeStringMap[type] + "\033[0m");
	else
		pattern = StringHelper::ReplaceAll(pattern, "{type}", _typeStringMap[type]);
	pattern = StringHelper::ReplaceAll(pattern, "{date}", CurrentDate());
	pattern = StringHelper::ReplaceAll(pattern, "{time}", CurrentTime());
	pattern = StringHelper::ReplaceAll(pattern, "{message}", message);

	return pattern;
}

void Logger::Log(LogType type, std::string message, bool ignoreDuplicants)
{
	if (_options.ignoredTypes[_typeStringMap[type]]) return; //exit if type is ignored
	if (ignoreDuplicants && _lastMessage == message) return;

	_lastMessage = message;

	if (_options.outputStream == LogOptions::LogOutput::FILE || _options.outputStream == LogOptions::LogOutput::BOTH)
	{
		if (ignoreDuplicants) message += "  [IGNORE DUPLICANTS]";
		_outputStream << MessageBuilder(type, message, false) << std::endl;
	}
	if (_options.outputStream == LogOptions::LogOutput::CONSOLE || _options.outputStream == LogOptions::LogOutput::BOTH)
	{
		if (ignoreDuplicants) message += "  \033[33m[IGNORE DUPLICANTS]\033[0m";
		std::cout << MessageBuilder(type, message, true) << std::endl;
	}
}

std::string Logger::CurrentDate()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	std::string out(buf);
	return out;
}

std::string Logger::CurrentTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	std::string out(buf);
	return out;
}


Stopwatch* Stopwatch::_instance = nullptr;

Stopwatch* Stopwatch::GetInstance()
{
	if (_instance == nullptr) _instance = new Stopwatch();
	return _instance;
}

void Stopwatch::Start(std::string name)
{
	_stopwatches[name] = std::chrono::high_resolution_clock::now();
}

std::chrono::microseconds Stopwatch::Stop(std::string name)
{
	std::chrono::microseconds output = std::chrono::duration_values<std::chrono::microseconds>::zero();

	auto found = _stopwatches.find(name);
	if (found != _stopwatches.end())
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = now - found->second;
		output = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		_stopwatches.erase(found);
	}
	return output;
}
