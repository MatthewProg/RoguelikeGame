#pragma once

#include "../Core/Logger.h"

#include "SFML/Graphics/Font.hpp"

class FontsManager
{
private:
	std::map<std::string, sf::Font> _fonts;
	unsigned short _expectedSize; //for logging purposes

	Logger* _logger;
public:
	FontsManager();
	~FontsManager();

	void LoadFromFile(const std::string& name, const std::string& path);
	void LoadFromMemory(const std::string& name, const void* data, size_t size);
	void LoadFromStream(const std::string& name, sf::InputStream& stream);

	sf::Font* GetFont(std::string name);

	bool Exists(std::string name);

	void SetExpectedSize(unsigned short size);
};

