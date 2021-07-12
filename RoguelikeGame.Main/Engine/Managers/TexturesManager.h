#pragma once

#include <map>
#include <memory>

#include "../Helpers/CollisionHelper.h"
#include "../Core/Logger.h"

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Image.hpp"

class TexturesManager
{
private:
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, std::shared_ptr<sf::Texture>> _tmpTextures;

	Logger* _logger;
public:
	TexturesManager();
	~TexturesManager();

	void LoadFromFile(const std::string& name, const std::string& path, const sf::IntRect& area = sf::IntRect());
	void LoadFromImage(const std::string& name, const sf::Image& img, const sf::IntRect& area = sf::IntRect());
	void LoadFromMemory(const std::string& name, const void* data, size_t size, const sf::IntRect& area = sf::IntRect());
	void LoadFromStream(const std::string& name, sf::InputStream& stream, const sf::IntRect& area = sf::IntRect());

	std::shared_ptr<sf::Texture> CreateTmpTexture(const std::string& name, const std::string& source, const sf::IntRect& area = sf::IntRect());

	sf::Texture* GetTexture(const std::string& name);
	std::shared_ptr<sf::Texture> GetTmpTexture(const std::string& name);
	
	bool Exists(const std::string& name) const;
	bool TmpExists(const std::string& name) const;

	void ApplySmooth(bool smooth);
	void ApplyRepeat(bool repeat);
};

