#include "FontsManager.h"

FontsManager::FontsManager()
{
	_fonts.clear();
	_expectedSize = 0;

	_logger = Logger::GetInstance();
}

FontsManager::~FontsManager()
{
}

void FontsManager::LoadFromFile(const std::string& name, const std::string& path)
{
	std::string message = "Fonts ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_fonts.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_fonts.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_fonts[name].loadFromFile(path) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

void FontsManager::LoadFromMemory(const std::string& name, const void* data, size_t size)
{
	std::string message = "Fonts ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_fonts.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_fonts.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_fonts[name].loadFromMemory(data, size) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

void FontsManager::LoadFromStream(const std::string& name, sf::InputStream& stream)
{
	std::string message = "Fonts ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_fonts.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_fonts.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_fonts[name].loadFromStream(stream) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

sf::Font* FontsManager::GetFont(std::string name)
{
	auto found = _fonts.find(name);
	if (found != _fonts.end())
		return &((*found).second);
	else
		return nullptr;
}

bool FontsManager::Exists(std::string name)
{
	if (_fonts.find(name) != _fonts.end())
		return true;
	return false;
}

void FontsManager::SetExpectedSize(unsigned short size)
{
	_expectedSize = size;
}
