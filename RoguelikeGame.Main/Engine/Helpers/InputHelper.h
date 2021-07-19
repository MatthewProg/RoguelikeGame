#pragma once

#include <iostream>

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

class InputHelper
{
public:
	static std::string DecodeKey(sf::Keyboard::Key key);
	static std::string DecodeButton(sf::Mouse::Button button);

	static sf::Keyboard::Key EncodeKey(const std::string& key);
	static sf::Mouse::Button EncodeButton(const std::string& button);
};

