#include "FontsManager.h"

FontsManager::FontsManager()
{
	_fonts.clear();
	_logger = Logger::GetInstance();
}

void FontsManager::LoadFromFile(const std::string& name, const std::string& path)
{
	std::string message = " font No" + std::to_string(_fonts.size() + 1) + " (" + name + ") from \"" + path + "\"";

	if (_fonts[name].loadFromFile(path) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

void FontsManager::LoadFromMemory(const std::string& name, const void* data, size_t size)
{
	std::string message = " font No" + std::to_string(_fonts.size() + 1) + " (" + name + ") from memory";

	if (_fonts[name].loadFromMemory(data, size) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

void FontsManager::LoadFromStream(const std::string& name, sf::InputStream& stream)
{
	std::string message = " font No" + std::to_string(_fonts.size() + 1) + " (" + name + ") from stream";

	if (_fonts[name].loadFromStream(stream) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

const sf::Font* FontsManager::GetFont(const std::string& name)
{
	auto found = _fonts.find(name);
	if (found != _fonts.end())
		return &((*found).second);
	else
		return nullptr;
}

bool FontsManager::Exists(const std::string& name) const
{
	if (_fonts.find(name) != _fonts.end())
		return true;
	return false;
}
