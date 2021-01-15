#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "../Handlers/KeyboardEventHandler.h"
#include "../Core/EntityMovement.h"
#include "../Managers/ObjectsManager.h"
#include "../Helpers/DebugHelper.h"
#include "../Core/EnemiesAI.h"
#include "../Models/GameMap.h"
#include "../Models/Player.h"

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
	CollisionsManager _collisionsManager;

	Player _player;
	EntityMovement _playerMovement;
	EnemiesManager _enemies;
	EnemiesAI _enemiesAI;

	std::chrono::steady_clock::time_point _lastFrameTime;
	void SetDeltaAndTick();
	void RecalcPlayerRays();

	bool Tick();

	void Close();

#pragma region Events
	void ToggleGridVisibility();
	void ToggleActionMapVisibility();
	void ToggleHitboxVisibility();
	void ToggleConsoleInfo();
	void ToggleWeaponHitboxVisibility();
	void ToggleEnemiesHitboxVisibility();
	void ToggleMapCollisionLinesVisibility();
	void ToggleRaycastVisibility();
	void TogglePathfindingVisibility();
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

