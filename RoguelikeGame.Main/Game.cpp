#include "Game.h"


Game::Game(Logger* logger, sf::VideoMode vmode, std::string title)
{
	_logger = logger;
	_window.create(vmode, title);
	_window.setFramerateLimit(60);
	_delta = 1.0000000;
	_gameSpeed = 20;
	_lastFrameTime = std::chrono::steady_clock::now();
}

void Game::SetDelta()
{
	std::chrono::steady_clock::time_point newFrame = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(newFrame - _lastFrameTime).count();
	_lastFrameTime = newFrame;

	_delta = (dur / 1000000000.000) * _gameSpeed;
#ifndef NDEBUG
	_window.setTitle("FPS: " + std::to_string(1000000000 / dur));
#endif

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
	SetDelta();
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
