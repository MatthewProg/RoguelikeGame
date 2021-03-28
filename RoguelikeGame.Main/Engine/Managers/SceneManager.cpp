#include "SceneManager.h"

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if (_loadedScene == "") return;

	auto found = _scenes.find(_loadedScene);
	if (found == _scenes.end()) return;
	else if (found->second == nullptr) return;

	target.draw(*found->second);
	if (_showFocused)
		target.draw(_focusedOutline);
}

SceneManager::SceneManager()
{
	_logger = Logger::GetInstance();
	_scenes.clear();
	_loadedScene = "";
	_showFocused = false;
	_focusedOutline.setPrimitiveType(sf::LinesStrip);
	_focusedOutline.resize(5);
	for (uint8_t i = 0; i < 5; i++)
	{
		_focusedOutline[i].color = sf::Color::Blue;
		_focusedOutline[i].position = sf::Vector2f(-1, -1);
	}
}

SceneManager::~SceneManager()
{
	for (auto& p : _scenes)
		if (p.second != nullptr)
			delete p.second;
}

void SceneManager::UpdateFocus(const sf::Vector2f& mousePos, bool clicked)
{
	if (_loadedScene == "") return;

	auto loaded = _scenes.find(_loadedScene);
	if (loaded == _scenes.end()) return;
	else if (loaded->second == nullptr) return;

	loaded->second->UpdateFocus(mousePos, clicked);

	if (_showFocused)
	{
		auto focus = loaded->second->GetFocused();
		if (std::get<1>(focus) != nullptr)
		{
			auto bounds = std::get<1>(focus)->GetGlobalBounds();
			_focusedOutline[0].position = sf::Vector2f(bounds.left, bounds.top);
			_focusedOutline[1].position = sf::Vector2f(bounds.left + bounds.width, bounds.top);
			_focusedOutline[2].position = sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
			_focusedOutline[3].position = sf::Vector2f(bounds.left, bounds.top + bounds.height);
			_focusedOutline[4].position = sf::Vector2f(bounds.left, bounds.top);
		}
		else
		{
			for (uint8_t i = 0; i < 5; i++)
				_focusedOutline[i].position = sf::Vector2f(-1, -1);
		}
	}
}

void SceneManager::UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	if (_loadedScene == "") return;

	auto loaded = _scenes.find(_loadedScene);
	if (loaded == _scenes.end()) return;
	else if (loaded->second == nullptr) return;

	loaded->second->UpdateEvent(ev, mousePos);
}

void SceneManager::Update(bool tick, float delta)
{
	if (_loadedScene == "") return;

	auto loaded = _scenes.find(_loadedScene);
	if (loaded == _scenes.end()) return;
	else if (loaded->second == nullptr) return;

	loaded->second->Update(tick, delta);
}

void SceneManager::SetShowFocused(bool show)
{
	_showFocused = show;
}

void SceneManager::ToggleShowFocused()
{
	std::string status = (!_showFocused) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Toggle show UI frames: " + status);
	SetShowFocused(!_showFocused);
}

bool SceneManager::GetShowFocused() const
{
	return _showFocused;
}

void SceneManager::LoadScene(const std::string& name)
{
	auto found = _scenes.find(name);
	if (found != _scenes.end())
	{
		_loadedScene = name;
		if (found->second != nullptr)
			found->second->ClearFocus();
		_logger->Log(Logger::LogType::DEBUG, "Loaded scene: " + name);
	}
	else
		_logger->Log(Logger::LogType::ERROR, "Unable to load scene: " + name);
}

void SceneManager::UnloadCurrentScene()
{
	_loadedScene = "";
}

const std::string& SceneManager::GetLoadedSceneName() const
{
	return _loadedScene;
}

Scene* SceneManager::GetLoadedScene()
{
	return GetScene(_loadedScene);
}

void SceneManager::AddScene(const std::string& name, Scene* element)
{
	auto found = _scenes.find(name);
	if (found != _scenes.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_scenes.erase(found);
	}

	_scenes[name] = element;
}


void SceneManager::RemoveScene(const std::string& name)
{
	auto found = _scenes.find(name);
	if (found != _scenes.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_scenes.erase(found);
	}
}

Scene* SceneManager::GetScene(const std::string& name)
{
	auto found = _scenes.find(name);
	if (found != _scenes.end())
		return found->second;
	else
		return nullptr;
}

size_t SceneManager::GetNoOfScenes() const
{
	return _scenes.size();
}
