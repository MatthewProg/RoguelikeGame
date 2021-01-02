#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "KeyboardEventHandler.h"
#include "TransformAnimation.h"
#include "CollisionHelper.h"
#include "ObjectsManager.h"
#include "DebugHelper.h"
#include "ViewHelper.h"
#include "GameMap.h"
#include "Player.h"

class Game
{
private:
	Logger* _logger;
	DebugHelper _debug;

	double _delta = 1.000000000;
	double _tickCounter = 0.0;
	unsigned int _gameSpeed = 20;

	sf::RenderWindow _window;
	sf::View _camera;

	sf::Event _event;
	KeyboardEventHandler<Game> _keyboardHandler;

	TexturesManager _textures;
	ObjectsManager _objTemplates;
	GameMap<unsigned char> _gameMap;

	Player _player;

	std::chrono::steady_clock::time_point _lastFrameTime;
	void SetDeltaAndTick();

	bool Tick();

	void Close();

#pragma region Events
	void ToggleGridVisibility();
	void ToggleActionMapVisibility();
	void ToggleHitboxVisibility();
	void ToggleConsoleInfo();
	void ToggleWeaponHitboxVisibility();
#pragma endregion

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

