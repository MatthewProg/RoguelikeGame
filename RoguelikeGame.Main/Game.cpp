#include "Game.h"


Game::Game(sf::VideoMode vmode, std::string title)
{
	_logger = Logger::GetInstance();
	_camera.setCenter(0, 0);
	_camera.setSize(vmode.width / 4, vmode.height / 4);
	_window.create(vmode, title);
	_window.setFramerateLimit(60);
	//_window.setView(_camera);
	_delta = 1.0000000;
	_tickCounter = 0.0;
	_gameSpeed = 60;
	_lastFrameTime = std::chrono::steady_clock::now();
	_drawHitboxes = false;
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

#ifndef NDEBUG
	_window.setTitle("FPS: " + std::to_string(1000000000 / dur));
#endif

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
	_logger->Log(Logger::LogType::INFO, "Loading game components");
	if (_gameTiles["tiles1"].loadFromFile("./res/img/tiles.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (1/4): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (1/4): OK");
	if (_gameTiles["tiles2"].loadFromFile("./res/img/tiles2.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (2/4): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (2/4): OK");
	if (_gameTiles["players"].loadFromFile("./res/img/players.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (3/4): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (3/4): OK");
	if (_gameTiles["special"].loadFromFile("./res/img/special.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (4/4): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (4/4): OK");


	//Game map
	_logger->Log(Logger::LogType::INFO, "Loading map components");
	if(_gameMap.LoadFromFile("./res/maps/map1.json") == false)
		_logger->Log(Logger::LogType::ERROR, "Data (1/1): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Data (1/1): OK");

	//_gameMap.SetActionMapVisibility(true);
	_gameMap.SetActionMapOpacity(0.12);

	auto mapTiles = _gameMap.GetLayersTilesNames();
	for (size_t tile = 0; tile < mapTiles.size(); tile++)
	{
		auto exist = _gameTiles.find(mapTiles[tile]);
		if (exist == _gameTiles.end())
		{
			_logger->Log(Logger::LogType::ERROR, "Graphics (" + std::to_string(tile + 1) + "/" + std::to_string(mapTiles.size()) + "): ERROR");
			continue;
		}
		else
		{
			if (_gameMap.SetTilesTexture(mapTiles[tile], _gameTiles[mapTiles[tile]]) == false)
				_logger->Log(Logger::LogType::ERROR, "Graphics (" + std::to_string(tile + 1) + "/" + std::to_string(mapTiles.size()) + "): ERROR");
			else
				_logger->Log(Logger::LogType::INFO, "Graphics (" + std::to_string(tile + 1) + "/" + std::to_string(mapTiles.size()) + "): OK");
		}
	}
		


	//Player
	_logger->Log(Logger::LogType::INFO, "Loading player components");

	sf::AnimationContainer playerAnimations;
	sf::Animation idle;
	if(!idle.LoadFromImage(_gameTiles["players"]))
		_logger->Log(Logger::LogType::ERROR, "Graphics (1/2): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (1/2): OK");
	idle.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 8));
	idle.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 9));
	idle.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 10));
	idle.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 11));
	idle.SetChangeFrameEvery(7);
	
	sf::Animation move;
	if (!move.LoadFromImage(_gameTiles["players"]))
		_logger->Log(Logger::LogType::ERROR, "Graphics (2/2): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (2/2): OK");
	move.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 12));
	move.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 13));
	move.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 14));
	move.AddNewFrame(TilesHelper::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 15));
	move.SetChangeFrameEvery(3);

	playerAnimations.SetStateAnimation("idle", idle);
	playerAnimations.SetStateAnimation("move", move);
	playerAnimations.ApplySetScale(1, 1);
	playerAnimations.ApplySetSmooth(false);

	_player.SetPlayerAnimations(playerAnimations);
	_player.SetPlayerState("idle");
	_player.SetCollisionBoxOffset(sf::FloatRect(3, 6, 9, 15));

	//Debug
	_drawHitboxes = true;
}

void Game::EventUpdate()
{
	while (_window.pollEvent(_event))
	{
		if (_event.type == sf::Event::Closed)
			Close();	
		if (_event.type == sf::Event::KeyPressed)
		{
			_player.SetPlayerState("move");
			_player.MoveBy(10, 0, _delta);
		}
		if(_event.type == sf::Event::KeyReleased)
			_player.SetPlayerState("idle");
	}
}

void Game::Update()
{
	SetDeltaAndTick();
	_player.Tick(Game::Tick());
	//_camera.setCenter(ViewHelper::GetRectCenter(_player.GetCollisionBox()));
	//_window.setView(_camera);
}

void Game::Clear()
{
	_window.clear();
}

void Game::Draw()
{
	_window.draw(_gameMap);
	_window.draw(_player);
	if (_drawHitboxes)
		_window.draw(_player.GetHitboxRect());
}

void Game::Display()
{
	_window.display();
}

bool Game::isRunning()
{
	return _window.isOpen();
}
