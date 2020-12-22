#include "Utilities.h"

void Utilities::Generate(unsigned int width, unsigned int height)
{
	std::vector<sf::Uint8> pixels;
	for (unsigned int i = 0; i < width * height; i++)
	{
		if (!(i / width >= height / 2) != !(i % width >= width / 2))
		{
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(255));
		}
		else
		{
			pixels.push_back(sf::Uint8(255));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(192));
			pixels.push_back(sf::Uint8(255));
		}
	}

	sf::Image img;
	img.create(width, height, pixels.data());
	_noTxt16x16.loadFromImage(img);
}

sf::Texture* Utilities::NoTexture16x16()
{
	return &_noTxt16x16;
}

Utilities* Utilities::GetInstance()
{
	if (_utilities == nullptr) {
		_utilities = new Utilities();
	}
	return _utilities;
}

Utilities* Utilities::_utilities = nullptr;
