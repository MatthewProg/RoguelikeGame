#include "Logger.h"

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

void Logger::Log(LogType type, std::string message)
{
	if (_options.ignoredTypes[_typeStringMap[type]]) return; //exit if type is ignored

	if (_options.outputStream == LogOptions::LogOutput::FILE || _options.outputStream == LogOptions::LogOutput::BOTH)
	{
		_outputStream << MessageBuilder(type, message, false) << std::endl;
	}
	if (_options.outputStream == LogOptions::LogOutput::CONSOLE || _options.outputStream == LogOptions::LogOutput::BOTH)
	{
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
