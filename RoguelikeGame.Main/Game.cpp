#include "Game.h"


Game::Game(sf::VideoMode vmode, std::string title)
{
	_logger = Logger::GetInstance();
	_window.create(vmode, title);
	_window.setFramerateLimit(60);
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
		_logger->Log(Logger::LogType::ERROR, "Graphics (1/3): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (1/3): OK");
	if (_gameTiles["tiles2"].loadFromFile("./res/img/tiles2.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (2/3): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (2/3): OK");
	if (_gameTiles["players"].loadFromFile("./res/img/players.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Graphics (3/3): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (3/3): OK");



	//Player
	_logger->Log(Logger::LogType::INFO, "Loading player components");

	sf::AnimationContainer playerAnimations;
	sf::Animation idle;
	if(!idle.LoadFromImage(_gameTiles["players"]))
		_logger->Log(Logger::LogType::ERROR, "Graphics (1/2): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (1/2): OK");
	idle.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 8));
	idle.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 9));
	idle.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 10));
	idle.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 11));
	idle.SetChangeFrameEvery(7);
	
	sf::Animation move;
	if (!move.LoadFromImage(_gameTiles["players"]))
		_logger->Log(Logger::LogType::ERROR, "Graphics (2/2): ERROR");
	else
		_logger->Log(Logger::LogType::INFO, "Graphics (2/2): OK");
	move.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 12));
	move.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 13));
	move.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 14));
	move.AddNewFrame(TilesManager::GetTileRect(_gameTiles["players"].getSize(), 16, 22, 15));
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
	}
}

void Game::Update()
{
	SetDeltaAndTick();
	_player.Tick(Game::Tick());
}

void Game::Clear()
{
	_window.clear();
}

void Game::Draw()
{
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
