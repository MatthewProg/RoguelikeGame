#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window.hpp"

#include "../Handlers/KeyboardEventHandler.h"
#include "../Managers/ObjectsManager.h"
#include "../Managers/SoundsManager.h"
#include "../Managers/FontsManager.h"
#include "../Managers/SceneManager.h"
#include "../Helpers/DebugHelper.h"
#include "../Core/EntityMovement.h"
#include "../Core/EnemiesAI.h"
#include "../Models/GameMap.h"
#include "../Models/Player.h"

class Game
{
private:
	Logger* _logger;
	DebugHelper _debug;

	double _delta;
	double _tickCounter;
	unsigned int _gameSpeed;
	bool _inGame;

	sf::RenderWindow _window;
	sf::View _camera;
	sf::View _gui;

	sf::Event _event;
	KeyboardEventHandler<Game> _keyboardHandler;

	CollisionsManager _collisionsManager;
	ObjectsManager _objTemplates;
	TexturesManager _textures;
	SoundsManager _sounds;
	FontsManager _fonts;
	GameMap<unsigned char> _gameMap;

	SceneManager _sceneManager;

	Player* _player;
	EntityMovement _playerMovement;
	EnemiesManager _enemies;
	EnemiesAI _enemiesAI;
	std::chrono::steady_clock::time_point _lastFrameTime;
	void SetDeltaAndTick();
	void RecalcPlayerRays();
	void UpdateUI();
	void UpdateGame();
	void CheckButtons();
	void LoadLevel(std::string path, std::string playerTemplate);

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
	void ToggleNoClip();
	void ToggleUIFrames();
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

