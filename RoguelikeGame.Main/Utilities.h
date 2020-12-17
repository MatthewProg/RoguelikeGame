#pragma once

#include <vector>
#include "SFML/Graphics/Image.hpp"

class Utilities
{
protected:
	Utilities() { _noTxt16x16 = Generate(16, 16); }
	static Utilities* _utilities;
private:
	sf::Image _noTxt16x16;
	sf::Image Generate(unsigned int width, unsigned int height);
public:
	sf::Image NoTexture16x16();

	Utilities(Utilities& other) = delete;
	void operator=(const Utilities&) = delete;

	static Utilities* GetInstance();
};

