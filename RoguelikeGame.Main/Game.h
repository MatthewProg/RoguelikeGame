#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Logger.h"
#include "TilesManager.h"

class Game
{
private:
	Logger* _logger;

	double _delta = 1.000000000;
	double _tickCounter = 0.0;
	unsigned int _gameSpeed = 20;
	sf::RenderWindow _window;
	sf::Event _event;

	sf::Image _gameTiles;

	std::chrono::steady_clock::time_point _lastFrameTime;
	void SetDeltaAndTick();

	bool Tick();

	void Close();
public:
	Game(sf::VideoMode vmode, std::string title);
	~Game();

	void Start();
	void EventUpdate();
	void Update();

	void Clear();
	void Draw();
	void Display();

	bool isRunning();
};

