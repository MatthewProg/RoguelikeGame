#pragma once

#include <vector>
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"

class Utilities
{
protected:
	Utilities() { Generate(16, 16); }
	static Utilities* _utilities;
private:
	sf::Texture _noTxt16x16;
	void Generate(unsigned int width, unsigned int height);
public:
	sf::Texture* NoTexture16x16();

	Utilities(Utilities& other) = delete;
	void operator=(const Utilities&) = delete;

	static Utilities* GetInstance();
};

