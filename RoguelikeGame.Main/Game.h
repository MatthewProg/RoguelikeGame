#pragma once

#include <chrono>

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "ViewHelper.h"
#include "GameMap.h"
#include "Player.h"

class Game
{
private:
	Logger* _logger;

	bool _drawHitboxes;

	double _delta = 1.000000000;
	double _tickCounter = 0.0;
	unsigned int _gameSpeed = 20;

	sf::RenderWindow _window;
	sf::Event _event;
	sf::View _camera;

	std::map<std::string, sf::Image> _gameTiles;
	GameMap<unsigned char> _gameMap;

	Player _player;

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

