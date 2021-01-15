#pragma once

#include <map>

#include "../Core/Logger.h"

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Image.hpp"

class TexturesManager
{
private:
	std::map<std::string, sf::Texture> _textures;
	unsigned short _expectedSize; //for logging purposes

	Logger* _logger;
public:
	TexturesManager();
	~TexturesManager();

	void LoadFromFile(const std::string& name, const std::string& path, const sf::IntRect& area = sf::IntRect());
	void LoadFromImage(const std::string& name, const sf::Image& img, const sf::IntRect& area = sf::IntRect());
	void LoadFromMemory(const std::string& name, const void* data, size_t size, const sf::IntRect& area = sf::IntRect());
	void LoadFromStream(const std::string& name, sf::InputStream& stream, const sf::IntRect& area = sf::IntRect());

	sf::Texture* GetTexture(std::string name);
	
	bool Exists(std::string name);

	void SetExpectedSize(unsigned short size);

	void ApplySmooth(bool smooth);
	void ApplyRepeat(bool repeat);
};

