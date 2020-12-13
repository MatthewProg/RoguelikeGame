#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>

#include "Logger.h"

class Game
{
private:
	Logger* _logger;

	double _delta = 1.000000000;
	unsigned int _gameSpeed = 20;
	sf::RenderWindow _window;
	sf::Event _event;

	std::chrono::steady_clock::time_point _lastFrameTime;
	void SetDelta();

	void Close();

public:
	Game(Logger* logger, sf::VideoMode vmode, std::string title);
	~Game();

	void Start();
	void EventUpdate();
	void Update();

	void Clear();
	void Draw();
	void Display();

	bool isRunning();
};

