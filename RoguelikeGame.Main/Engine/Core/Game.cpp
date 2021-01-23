#include "Game.h"


Game::Game(sf::VideoMode vmode, std::string title) : _keyboardHandler(this)
{
	_logger = Logger::GetInstance();
	_camera.setCenter(128, 64);
	_camera.setSize((float)vmode.width / 4, (float)vmode.height / 4);
	_window.create(vmode, title);
	//_window.setFramerateLimit(144);
	_window.setView(_camera);
	_delta = 1.0000000;
	_tickCounter = 0.0;
	_gameSpeed = 60;
	_lastFrameTime = std::chrono::steady_clock::now();
	_event = sf::Event();
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
	auto angle = MathHelper::GetAngleBetweenPoints(ViewHelper::GetRectCenter(_player.GetCollisionBox()), sf::Vector2f(coord.x, coord.y));
	_player.GetWeapon()->SetCurrentAngle(angle);
	_player.GetWeapon()->SetRaycastHitpoint(_collisionsManager.GetRayHitpoint(ViewHelper::GetRectCenter(_player.GetCollisionBox()), angle, 500));
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
}

void Game::Start()
{
	_logger->Log(Logger::LogType::INFO, "Starting...");

	//Game
	_textures.SetExpectedSize(4);
	_textures.LoadFromFile("tiles1", "./res/img/tiles.png");
	_textures.LoadFromFile("tiles2", "./res/img/tiles2.png");
	_textures.LoadFromFile("players", "./res/img/players.png");
	_textures.LoadFromFile("special", "./res/img/special.png");
	_textures.ApplySmooth(false);
	_textures.ApplyRepeat(false);

	//Object manager
	_objTemplates.SetTexturesManager(&_textures);

	//Game map
	_logger->Log(Logger::LogType::INFO, "Loading map components");
	if (_gameMap.LoadFromFile("./res/maps/map1.json") == false)
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

	sf::AnimationContainer playerAnimations;

	sf::Animation idle;
	idle.SetTexture(_textures.GetTexture("players"));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 8));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 9));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 10));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 11));
	idle.SetChangeFrameEvery(7);

	sf::Animation move;
	move.SetTexture(_textures.GetTexture("players"));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 12));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 13));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 14));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures.GetTexture("players")->getSize(), 16, 22, 15));
	move.SetChangeFrameEvery(3);

	playerAnimations.SetStateAnimation("idle", idle);
	playerAnimations.SetStateAnimation("move", move);
	playerAnimations.ApplySetScale(1, 1);

	_player.SetAnimations(playerAnimations);
	_player.SetState("idle");
	_player.SetCollisionBoxOffset(sf::FloatRect(3, 6, 9, 15));
	_player.SetPosition(sf::Vector2f(296, 472));
	_player.SetWeapon(_objTemplates.GetMeleeWeapon("sword"));
	auto camSize = _camera.getSize() + sf::Vector2f(44, 44);
	_player.SetView(sf::FloatRect(0 - (camSize.x / 2), 0 - (camSize.y / 2), camSize.x, camSize.y));

	//Player movement
	_playerMovement.SetIdleStateName("idle");
	_playerMovement.SetMoveStateName("move");
	_playerMovement.SetEntity(&_player);
	_playerMovement.SetCollisionsManager(&_collisionsManager);

	//Enemies AI
	_enemiesAI.SetTarget(&_player);
	_enemiesAI.SetCollisionsManager(&_collisionsManager);
	_enemiesAI.SetEnemiesManager(&_enemies);
	_enemiesAI.SetPathfindPoints(_gameMap.GetPathfindingPoints());

	//Enemies
	_enemies.SetPlayer(&_player);
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	//
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	//
	_enemies.GetEnemies()->at(0)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	/*_enemies.GetEnemies()->at(1)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(2)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(3)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));*/
	_enemies.GetEnemies()->at(0)->SetPosition(500, 290);
	_enemies.GetEnemies()->at(1)->SetPosition(510, 290);
	_enemies.GetEnemies()->at(2)->SetPosition(520, 290);
	_enemies.GetEnemies()->at(3)->SetPosition(530, 290);
	_enemies.GetEnemies()->at(4)->SetPosition(540, 290);
	_enemies.GetEnemies()->at(5)->SetPosition(550, 290);
	_enemies.GetEnemies()->at(6)->SetPosition(560, 290);
	_enemies.GetEnemies()->at(7)->SetPosition(570, 290);
	_enemies.GetEnemies()->at(8)->SetPosition(580, 290);
	_enemies.GetEnemies()->at(9)->SetPosition(590, 290);
	_enemies.GetEnemies()->at(10)->SetPosition(500, 310);
	_enemies.GetEnemies()->at(11)->SetPosition(510, 310);
	_enemies.GetEnemies()->at(12)->SetPosition(520, 310);
	_enemies.GetEnemies()->at(13)->SetPosition(530, 310);
	_enemies.GetEnemies()->at(14)->SetPosition(540, 310);
	_enemies.GetEnemies()->at(15)->SetPosition(550, 310);
	_enemies.GetEnemies()->at(16)->SetPosition(560, 310);
	_enemies.GetEnemies()->at(17)->SetPosition(570, 310);
	_enemies.GetEnemies()->at(18)->SetPosition(580, 310);
	_enemies.GetEnemies()->at(19)->SetPosition(590, 310);
	/*_enemies.GetEnemies()->at(1)->SetPosition(580, 300);
	_enemies.GetEnemies()->at(2)->SetPosition(590, 310);
	_enemies.GetEnemies()->at(3)->SetPosition(610, 300);*/

	//Debug
	_gameMap.SetActionMapOpacity(0.25);

	sf::Event::KeyEvent ctrlAltG = { sf::Keyboard::G, true, true };
	sf::Event::KeyEvent ctrlAltA = { sf::Keyboard::A, true, true };
	sf::Event::KeyEvent ctrlAltH = { sf::Keyboard::H, true, true };
	sf::Event::KeyEvent ctrlAltD = { sf::Keyboard::D, true, true };
	sf::Event::KeyEvent ctrlAltR = { sf::Keyboard::R, true, true };
	sf::Event::KeyEvent ctrlAltP = { sf::Keyboard::P, true, true };
	_keyboardHandler.NewOn(ctrlAltG, &Game::ToggleGridVisibility);
	_keyboardHandler.NewOn(ctrlAltA, &Game::ToggleActionMapVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleEnemiesHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleMapCollisionLinesVisibility);
	_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleWeaponHitboxVisibility);
	_keyboardHandler.NewOn(ctrlAltD, &Game::ToggleConsoleInfo);
	_keyboardHandler.NewOn(ctrlAltR, &Game::ToggleRaycastVisibility);
	_keyboardHandler.NewOn(ctrlAltP, &Game::TogglePathfindingVisibility);

	//Reset timings
	SetDeltaAndTick();
}

void Game::EventUpdate()
{
	while (_window.pollEvent(_event))
	{
		if (_event.type == sf::Event::Closed)
			Close();	
		if (_event.type == sf::Event::KeyPressed)
			_keyboardHandler.Rise(_event.key);
		if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
		{
			if (_player.GetWeapon()->CanAttack())
			{
				_player.GetWeapon()->Attack();
				_enemies.CheckForHit();
			}
		}
		if (_event.type == sf::Event::MouseMoved)
			RecalcPlayerRays();
	}
}

void Game::Update()
{
	SetDeltaAndTick();
	_debug.Status(Game::Tick());

	//Stopwatch::GetInstance()->Start("player");
	_playerMovement.Update((float)_delta);
	_player.Update(Game::Tick(), (float)_delta);
	//auto pl = std::to_string(Stopwatch::GetInstance()->Stop("player").count()) + "us";

	//Stopwatch::GetInstance()->Start("rays");
	if (_playerMovement.IsKeyPressed()) RecalcPlayerRays();
	//auto rays = std::to_string(Stopwatch::GetInstance()->Stop("rays").count()) + "us";

	//Stopwatch::GetInstance()->Start("enemies");
	_enemies.Update(Game::Tick(), (float)_delta);
	//auto en = std::to_string(Stopwatch::GetInstance()->Stop("enemies").count()) + "us";
	_enemies.CheckAttacks();

	//Stopwatch::GetInstance()->Start("ai");
	_enemiesAI.Update((float)_delta);
	//auto ai = std::to_string(Stopwatch::GetInstance()->Stop("ai").count()) + "us";
	//_logger->Log(Logger::LogType::DEBUG, "Player: "+pl+"   Rays: "+rays+"   Enemies: "+en+"   AI: "+ai);
	
	_camera.setCenter(ViewHelper::GetRectCenter(_player.GetCollisionBox()));
	_window.setView(_camera);
}

void Game::Clear()
{
	_window.clear();
}

void Game::Draw()
{
	_window.draw(_gameMap);
	_window.draw(_collisionsManager);
	_window.draw(_enemiesAI);
	_window.draw(_enemies);
	_window.draw(_player);
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
	_player.ToggleHitboxVisibility();
}

void Game::ToggleConsoleInfo()
{
	_debug.ToggleFPSInfo();
}

void Game::ToggleWeaponHitboxVisibility()
{
	_player.ToggleWeaponHitboxVisibility();
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
	_player.ToggleRaycastVisibility();
	_enemies.ToggleEnemiesRaycastVisibility();
}

void Game::TogglePathfindingVisibility()
{
	_enemiesAI.TogglePathfindingVisibility();
}

#pragma endregion

