#include "SceneManager.h"

void SceneManager::UpdateAllBoundsOutline()
{
	if (_loadedScene == "") return;

	auto found = _scenes.find(_loadedScene);
	if (found == _scenes.end()) return;
	else if (found->second == nullptr) return;

	_allOutline.clear();
	sf::Vertex v;
	v.color = sf::Color::Blue;

	for (auto& e : found->second->GetElements())
	{
		auto bounds = e.second->GetAllBoundsPoints();
		for (size_t i = 0; i < (bounds.size() / 4); i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				v.position = bounds[(i*4)+j];
				_allOutline.append(v);
				v.position = bounds[(i*4)+((j+1)%4)];
				_allOutline.append(v);
			}
		}
	}
}

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	if (_loadedScene == "") return;

	auto found = _scenes.find(_loadedScene);
	if (found == _scenes.end()) return;
	else if (found->second == nullptr) return;

	target.draw(*found->second);
	if (_showAllBounds)
		target.draw(_allOutline);
	if (_showFocused)
		target.draw(_focusedOutline);
}

SceneManager::SceneManager()
{
	_logger = Logger::GetInstance();
	_scenes.clear();
	_loadedScene = "";
	_showFocused = false;
	_showAllBounds = false;
	_focusedOutline.setPrimitiveType(sf::LinesStrip);
	_focusedOutline.resize(5);
	_allOutline.setPrimitiveType(sf::Lines);
	for (uint8_t i = 0; i < 5; i++)
	{
		_focusedOutline[i].color = sf::Color::Yellow;
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
			auto bounds = std::get<1>(focus)->GetDeepestInFocusBoundsPoints();
			for (uint8_t i = 0; i < 4; i++)
			{
				_focusedOutline[i].position = bounds[i];
				_focusedOutline[i + 1].position = bounds[(i + 1) % 4];
			}
		}
		else
		{
			for (uint8_t i = 0; i < 5; i++)
				_focusedOutline[i].position = sf::Vector2f(-1.f, -1.f);
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

	if (_showAllBounds == true && tick == true)
		UpdateAllBoundsOutline();
	loaded->second->Update(tick, delta);
}

void SceneManager::SetShowFocused(bool show)
{
	_showFocused = show;
}

void SceneManager::SetShowAllBounds(bool show)
{
	_showAllBounds = show;
	UpdateAllBoundsOutline();
}

void SceneManager::ToggleShowFocused()
{
	if (_showFocused == true && _showAllBounds == false)
	{
		SetShowFocused(true);
		SetShowAllBounds(true);
		_logger->Log(Logger::LogType::DEBUG, "Toggle show UI frames: all");
	}
	else if (_showFocused == true && _showAllBounds == true)
	{
		SetShowFocused(false);
		SetShowAllBounds(false);
		_logger->Log(Logger::LogType::DEBUG, "Toggle show UI frames: none");
	}
	else
	{
		SetShowFocused(true);
		SetShowAllBounds(false);
		_logger->Log(Logger::LogType::DEBUG, "Toggle show UI frames: focused");
	}
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
		UpdateAllBoundsOutline();
		_logger->Log(Logger::LogType::DEBUG, "Loaded scene: " + name);
	}
	else
		_logger->Log(Logger::LogType::ERROR, "Unable to load scene: " + name);
}

void SceneManager::UnloadCurrentScene()
{
	_loadedScene = "";
	UpdateAllBoundsOutline();
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
		if (name == _loadedScene)
			UnloadCurrentScene();

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
