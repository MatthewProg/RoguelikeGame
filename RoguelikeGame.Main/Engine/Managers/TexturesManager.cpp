#include "TexturesManager.h"

TexturesManager::TexturesManager()
{
	_logger = Logger::GetInstance();
	_expectedSize = 0;
}

TexturesManager::~TexturesManager()
{
}

void TexturesManager::LoadFromFile(const std::string& name, const std::string& path, const sf::IntRect& area)
{
	std::string message = "Graphics ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_textures.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_textures.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_textures[name].loadFromFile(path, area) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

void TexturesManager::LoadFromImage(const std::string& name, const sf::Image& img, const sf::IntRect& area)
{
	std::string message = "Graphics ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_textures.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_textures.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_textures[name].loadFromImage(img, area) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

void TexturesManager::LoadFromMemory(const std::string& name, const void* data, size_t size, const sf::IntRect& area)
{
	std::string message = "Graphics ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_textures.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_textures.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_textures[name].loadFromMemory(data, size, area) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

void TexturesManager::LoadFromStream(const std::string& name, sf::InputStream& stream, const sf::IntRect& area)
{
	std::string message = "Graphics ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_textures.size() + 1) + "): ";
	else
		message += "(" + std::to_string(_textures.size() + 1) + "/" + std::to_string(_expectedSize) + "): ";


	if (_textures[name].loadFromStream(stream, area) == false)
		_logger->Log(Logger::LogType::ERROR, message + "ERROR");
	else
		_logger->Log(Logger::LogType::INFO, message + "OK");
}

sf::Texture* TexturesManager::GetTexture(const std::string& name)
{
	auto found = _textures.find(name);
	if (found != _textures.end())
		return &((*found).second);
	else
		return nullptr;
}

bool TexturesManager::Exists(const std::string& name) const
{
	if (_textures.find(name) != _textures.end())
		return true;
	return false;
}

void TexturesManager::SetExpectedSize(unsigned short size)
{
	_expectedSize = size;
}

void TexturesManager::ApplySmooth(bool smooth)
{
	for (auto it = _textures.begin(); it != _textures.end(); it++)
		it->second.setSmooth(smooth);
}

void TexturesManager::ApplyRepeat(bool repeat)
{
	for (auto it = _textures.begin(); it != _textures.end(); it++)
		it->second.setRepeated(repeat);
}
