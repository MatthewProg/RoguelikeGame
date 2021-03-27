#include "Game.h"


Game::Game(sf::VideoMode vmode, std::string title) : _keyboardHandler(this)
{
	_logger = Logger::GetInstance();
	_camera.setCenter(128, 64);
	_camera.setSize((float)vmode.width / 4, (float)vmode.height / 4);
	_gui.setCenter(((float)vmode.width / 2.f), ((float)vmode.height / 2.f));
	_gui.setSize((float)vmode.width / 1, (float)vmode.height / 1);
	_gui.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	_window.create(vmode, title);
	//_window.setFramerateLimit(144);
	_window.setView(_camera);
	_delta = 1.0000000;
	_tickCounter = 0.0;
	_gameSpeed = 60;
	_inGame = false;
	_lastFrameTime = std::chrono::steady_clock::now();
	_event = sf::Event();
	_player = nullptr;
	srand((uint32_t)time(NULL));
}

void Game::SetDeltaAndTick()
{
	if (_tickCounter >= 1)
		_tickCounter -= 1;

	std::chrono::steady_clock::time_point newFrame = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(newFrame - _lastFrameTime).count();
	_lastFrameTime = newFrame;

	_delta = (dur / 1000000000.000) * _gameSpeed;
	_tickCounter += _delta;
}

void Game::RecalcPlayerRays()
{
	auto coord = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
	auto angle = MathHelper::GetAngleBetweenPoints(ViewHelper::GetRectCenter(_player->GetCollisionBox()), sf::Vector2f(coord.x, coord.y));
	_player->GetWeapon()->SetCurrentAngle(angle);
	_player->GetWeapon()->SetRaycastHitpoint(_collisionsManager.GetRayHitpoint(ViewHelper::GetRectCenter(_player->GetCollisionBox()), angle, 500));
}

void Game::UpdateUI()
{
	if (_inGame)
	{
		auto sc = _sceneManager.GetScene("game");
		ProgressBar* hb = (ProgressBar*)sc->GetElement("healthBar");
		hb->SetCurrentValue(_player->GetHealth());
	}
	CheckButtons();
}

void Game::UpdateGame()
{
	_playerMovement.Update((float)_delta);
	_player->Update(Game::Tick(), (float)_delta);
	if (_playerMovement.IsKeyPressed()) RecalcPlayerRays();

	_enemies.Update(Game::Tick(), (float)_delta);
	_enemies.CheckAttacks();
	_enemiesAI.Update((float)_delta);

	_camera.setCenter(ViewHelper::GetRectCenter(_player->GetCollisionBox()));
}

void Game::CheckButtons()
{
	auto loaded = _sceneManager.GetLoadedScene();
	if (loaded != nullptr)
	{
		if (_sceneManager.GetLoadedSceneName() == "main_menu")
		{
			if (((Button*)loaded->GetElement("exit"))->Clicked()) Close();
			else if (((Button*)loaded->GetElement("options"))->Clicked()) { ; }
			else if (((Button*)loaded->GetElement("play"))->Clicked()) { LoadLevel("./res/maps/map1.json", "male_elf"); }
		}
	}
}

void Game::LoadLevel(std::string path, std::string playerTemplate)
{
	_logger->Log(Logger::LogType::INFO, "Loading level: " + path);

	//Game map
	_logger->Log(Logger::LogType::INFO, "Loading map components");
	if (_gameMap.LoadFromFile(path) == false)
		_logger->Log(Logger::LogType::ERROR, "Data (1/1): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Data (1/1): OK");

	_gameMap.AutoSetTilesTextures(&_textures);
	_gameMap.PrepareFrame();

	//Collisions
	_collisionsManager.AddMap(*_gameMap.GetActionMap(), (unsigned char)1);
	_collisionsManager.GenerateCommonMap();
	_collisionsManager.CovertTilesIntoEdges();

	//Player
	_logger->Log(Logger::LogType::INFO, "Loading player components");
	_player = _objTemplates.GetPlayer(playerTemplate);
	auto camSize = _camera.getSize() + sf::Vector2f(44, 44);
	_player->SetView(sf::FloatRect(0 - (camSize.x / 2), 0 - (camSize.y / 2), camSize.x, camSize.y));

	//Player movement
	_playerMovement.SetIdleStateName("idle");
	_playerMovement.SetMoveStateName("move");
	_playerMovement.SetEntity(_player);
	_playerMovement.SetCollisionsManager(&_collisionsManager);

	//Enemies AI
	_enemiesAI.SetTarget(_player);
	_enemiesAI.SetCollisionsManager(&_collisionsManager);
	_enemiesAI.SetEnemiesManager(&_enemies);
	_enemiesAI.SetPathfindPoints(_gameMap.GetPathfindingPoints());

	//Enemies
	_enemies.SetPlayer(_player);
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.GetEnemies()->at(0)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(1)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(2)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(3)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(0)->SetPosition(500, 290);
	_enemies.GetEnemies()->at(1)->SetPosition(580, 300);
	_enemies.GetEnemies()->at(2)->SetPosition(590, 310);
	_enemies.GetEnemies()->at(3)->SetPosition(610, 300);

	//Sounds
	_sounds.LoadFromFile("entities_dmg4", "./res/sounds/entities/dmg4.wav");
	_sounds.LoadFromFile("entities_dmg5", "./res/sounds/entities/dmg5.wav");
	_sounds.LoadFromFile("entities_dmg6", "./res/sounds/entities/dmg6.wav");
	_sounds.LoadFromFile("weapons_swing1", "./res/sounds/weapons/swing1.wav");
	_sounds.LoadFromFile("weapons_swing2", "./res/sounds/weapons/swing2.wav");
	_sounds.LoadFromFile("weapons_swing3", "./res/sounds/weapons/swing3.wav");
	_sounds.LoadFromFile("weapons_bite1", "./res/sounds/weapons/bite1.wav");

	//Scene
	_sceneManager.LoadScene("game");

	_inGame = true;
	_logger->Log(Logger::LogType::INFO, "Done");
}

bool Game::Tick()
{
	if (_tickCounter >= 1)
		return true;
	return false;
}

void Game::Close()
{
	_logger->Log(Logger::LogType::INFO, "Closing...");
	_window.close();
}

Game::~Game()
{
	delete _player;
}

void Game::Start()
{
	_logger->Log(Logger::LogType::INFO, "Starting...");

	//Textures
	_textures.SetExpectedSize(4);
	_textures.LoadFromFile("tiles1", "./res/img/tiles.png");
	_textures.LoadFromFile("tiles2", "./res/img/tiles2.png");
	_textures.LoadFromFile("players", "./res/img/players.png");
	_textures.LoadFromFile("special", "./res/img/special.png");
	_textures.LoadFromFile("ui", "./res/img/ui.png");
	_textures.ApplySmooth(false);
	_textures.ApplyRepeat(false);

	//Fonts
	_fonts.SetExpectedSize(1);
	_fonts.LoadFromFile("menu", "./res/fonts/menu.ttf");

	//Sounds
	_sounds.SetExpectedSize(5);
	_sounds.LoadFromFile("ui_click", "./res/sounds/ui/click.wav");
	_sounds.LoadFromFile("ui_coin", "./res/sounds/ui/coin.wav");
	_sounds.LoadFromFile("ui_hit", "./res/sounds/ui/hit.wav");
	_sounds.LoadFromFile("ui_release", "./res/sounds/ui/release.wav");
	_sounds.LoadFromFile("ui_select", "./res/sounds/ui/select.wav");

	//Object manager
	_objTemplates.SetTexturesManager(&_textures);
	_objTemplates.SetFontsManager(&_fonts);
	_objTemplates.SetSoundsManager(&_sounds);
	_objTemplates.SetWindowSize(_window.getSize());

	//UI elements
	_sceneManager.AddScene("game", _objTemplates.GetScene("game"));
	_sceneManager.AddScene("main_menu", _objTemplates.GetScene("main_menu"));
	_sceneManager.LoadScene("main_menu");
	_sceneManager.SetShowFocused(false);

	//Debug
	_gameMap.SetActionMapOpacity(0.25);

	//Keybinds
	sf::Event::KeyEvent ctrlAltG = { sf::Keyboard::G, true, true };
	sf::Event::KeyEvent ctrlAltA = { sf::Keyboard::A, true, true };
	sf::Event::KeyEvent ctrlAltH = { sf::Keyboard::H, true, true };
	sf::Event::KeyEvent ctrlAltD = { sf::Keyboard::D, true, true };
	sf::Event::KeyEvent ctrlAltR = { sf::Keyboard::R, true, true };
	sf::Event::KeyEvent ctrlAltP = { sf::Keyboard::P, true, true };
	sf::Event::KeyEvent ctrlAltN = { sf::Keyboard::N, true, true };
	sf::Event::KeyEvent ctrlAltU = { sf::Keyboard::U, true, true };
	_keyboardHandler.NewOn(ctrlAltG, &Game::ToggleGridVisibility);
	_keyboardHandler.NewOn(ctrlAltA, &Game::ToggleActionMapVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleEnemiesHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleMapCollisionLinesVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleWeaponHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltD, &Game::ToggleConsoleInfo);
	_keyboardHandler.NewOn(ctrlAltR, &Game::ToggleRaycastVisibility);
	_keyboardHandler.NewOn(ctrlAltP, &Game::TogglePathfindingVisibility);
	_keyboardHandler.NewOn(ctrlAltN, &Game::ToggleNoClip);
	_keyboardHandler.NewOn(ctrlAltU, &Game::ToggleUIFrames);

	//Reset timings
	SetDeltaAndTick();
}

void Game::EventUpdate()
{
	bool LMB_Clicked = false;
	_window.setView(_gui);
	auto mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
	_window.setView(_camera);
	while (_window.pollEvent(_event))
	{
		if (_event.type == sf::Event::Closed)
			Close();	
		if (_event.type == sf::Event::KeyPressed)
			_keyboardHandler.Rise(_event.key);
		if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
		{
			if (_inGame)
			{
				if (_player->GetWeapon()->CanAttack())
				{
					_player->GetWeapon()->Attack();
					_enemies.CheckForHit();
				}
			}
			LMB_Clicked = true;
		}
		if (_event.type == sf::Event::MouseMoved)
		{
			if (_inGame)
				RecalcPlayerRays();
		}

		_sceneManager.UpdateEvent(&_event, mousePos);
	}
	_sceneManager.UpdateFocus(mousePos, LMB_Clicked);
}

void Game::Update()
{
	SetDeltaAndTick();
	_debug.Status(Game::Tick());

	if (_inGame)
		UpdateGame();

	UpdateUI();
	_sceneManager.Update(Game::Tick(), (float)_delta);
	_sounds.Update();
}

void Game::Clear()
{
	_window.clear(sf::Color::Transparent);
}

void Game::Draw()
{
	if (_inGame)
	{
		_window.setView(_camera);
		_window.draw(_gameMap);
		_window.draw(_collisionsManager);
		_window.draw(_enemiesAI);
		_window.draw(_enemies);
		_window.draw(*_player);
	}

	_window.setView(_gui);
	_window.draw(_sceneManager);

	_window.setView(_camera);
}

void Game::Display()
{
	_window.display();
}

bool Game::isRunning()
{
	return _window.isOpen();
}

#pragma region Events
void Game::ToggleGridVisibility()
{
	_gameMap.ToggleGridVisibility();
}

void Game::ToggleActionMapVisibility()
{
	_gameMap.ToggleActionMapVisibility();
}

void Game::ToggleHitboxVisibility()
{
	_player->ToggleHitboxVisibility();
}

void Game::ToggleConsoleInfo()
{
	_debug.ToggleFPSInfo();
}

void Game::ToggleWeaponHitboxVisibility()
{
	_player->ToggleWeaponHitboxVisibility();
}

void Game::ToggleEnemiesHitboxVisibility()
{
	_enemies.ToggleEnemiesHitboxVisibility();
}

void Game::ToggleMapCollisionLinesVisibility()
{
	_collisionsManager.ToggleCollisionLinesVisibility();
}

void Game::ToggleRaycastVisibility()
{
	_player->ToggleRaycastVisibility();
	_enemies.ToggleEnemiesRaycastVisibility();
}

void Game::TogglePathfindingVisibility()
{
	_enemiesAI.TogglePathfindingVisibility();
}

void Game::ToggleNoClip()
{
	_playerMovement.ToggleNoClip();
}

void Game::ToggleUIFrames()
{
	_sceneManager.ToggleShowFocused();
}

#pragma endregion

