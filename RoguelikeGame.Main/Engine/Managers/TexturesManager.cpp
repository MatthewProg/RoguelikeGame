#include "TexturesManager.h"

TexturesManager::TexturesManager()
{
	_logger = Logger::GetInstance();
}

void TexturesManager::LoadFromFile(const std::string& name, const std::string& path, const sf::IntRect& area)
{
	std::string message = " graphics No" + std::to_string(_textures.size() + 1) + " (" + name + ") from \"" + path + "\"";

	if (_textures[name].loadFromFile(path, area) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

void TexturesManager::LoadFromImage(const std::string& name, const sf::Image& img, const sf::IntRect& area)
{
	std::string message = " graphics No" + std::to_string(_textures.size() + 1) + " (" + name + ") from image";

	if (_textures[name].loadFromImage(img, area) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

void TexturesManager::LoadFromMemory(const std::string& name, const void* data, size_t size, const sf::IntRect& area)
{
	std::string message = " graphics No" + std::to_string(_textures.size() + 1) + " (" + name + ") from memory";

	if (_textures[name].loadFromMemory(data, size, area) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

void TexturesManager::LoadFromStream(const std::string& name, sf::InputStream& stream, const sf::IntRect& area)
{
	std::string message = " graphics No" + std::to_string(_textures.size() + 1) + " (" + name + ") from stream";

	if (_textures[name].loadFromStream(stream, area) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	else
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
}

std::shared_ptr<sf::Texture> TexturesManager::CreateTmpTexture(const std::string& name, const std::string& source, const sf::IntRect& area)
{
	auto found = _tmpTextures.find(name);
	if (found != _tmpTextures.end() && found->second.use_count() > 1)
		return nullptr;

	auto tex = GetTexture(source);
	if (tex == nullptr)
		return nullptr;

	auto srcSize = tex->getSize();
	if (CollisionHelper::CheckRectContains(sf::IntRect(0, 0, int(srcSize.x), int(srcSize.y)), area) == false)
		return nullptr;

	//Find first with use_count <= 1
	std::shared_ptr<sf::Texture> empty = nullptr;
	for(auto it = _tmpTextures.begin(); it != _tmpTextures.end(); it++)
	{
		if (it->second.use_count() <= 1)
		{
			if (it->second == nullptr)
			{
				_tmpTextures.erase(it->first);
				continue;
			}
			else
			{
				it->second.swap(empty);
				_tmpTextures.erase(it->first);
				break;
			}
		}
	}

	//Convert
	auto img = tex->copyToImage();
	if (empty == nullptr)
		empty = std::make_shared<sf::Texture>();
	if (empty->loadFromImage(img, area) == false)
			return nullptr;
	_tmpTextures[name] = empty;

	return _tmpTextures[name];
}

sf::Texture* TexturesManager::GetTexture(const std::string& name)
{
	auto found = _textures.find(name);
	if (found != _textures.end())
		return &((*found).second);
	else
		return nullptr;
}

std::shared_ptr<sf::Texture> TexturesManager::GetTmpTexture(const std::string& name)
{
	auto found = _tmpTextures.find(name);
	if (found != _tmpTextures.end())
	{
		if (found->second.use_count() > 1)
			return found->second;
		else
			return nullptr;
	}
	return nullptr;
}

bool TexturesManager::Exists(const std::string& name) const
{
	if (_textures.find(name) != _textures.end())
		return true;
	return false;
}

bool TexturesManager::TmpExists(const std::string& name) const
{
	auto found = _tmpTextures.find(name);
	if (found != _tmpTextures.end())
	{
		if (found->second.use_count() > 1)
			return true;
		else
			return false;
	}
	return false;
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
