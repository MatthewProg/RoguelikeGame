#pragma once

#include "../Core/Logger.h"

#include "SFML/Graphics/Font.hpp"

class FontsManager
{
private:
	std::map<std::string, sf::Font> _fonts;

	Logger* _logger;
public:
	FontsManager();
	~FontsManager() = default;

	void LoadFromFile(const std::string& name, const std::string& path);
	void LoadFromMemory(const std::string& name, const void* data, size_t size);
	void LoadFromStream(const std::string& name, sf::InputStream& stream);

	const sf::Font* GetFont(const std::string& name);

	bool Exists(const std::string& name) const;
};

