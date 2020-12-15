#include "Game.h"


Game::Game(sf::VideoMode vmode, std::string title)
{
	_logger = Logger::GetInstance();
	_window.create(vmode, title);
	_window.setFramerateLimit(60);
	_delta = 1.0000000;
	_tickCounter = 0.0;
	_gameSpeed = 40;
	_lastFrameTime = std::chrono::steady_clock::now();
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
	_logger->Log(Logger::LogType::INFO, "Close() was called");
	_window.close();
}

Game::~Game()
{
}

void Game::Start()
{
	_logger->Log(Logger::LogType::INFO, "Start() was called");
	if (_gameTiles.loadFromFile("./res/img/tiles.png") == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load game tiles!");
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
}

void Game::Clear()
{
	_window.clear();
}

void Game::Draw()
{
}

void Game::Display()
{
	_window.display();
}

bool Game::isRunning()
{
	return _window.isOpen();
}
