#pragma once

#include <map>

#include "../UI/Scene.h"

#include "SFML/Graphics/Drawable.hpp"

class SceneManager : public sf::Drawable 
{
private:
	Logger* _logger;
	std::map<std::string, Scene*> _scenes;
	
	std::string _loadedScene;

	sf::VertexArray _focusedOutline;
	bool _showFocused;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	SceneManager();
	~SceneManager();

	void UpdateFocus(const sf::Vector2f& mousePos, bool clicked);
	void UpdateEvent(sf::Event* ev, sf::Vector2f mousePos);
	void Update(bool tick, float delta);

	void SetShowFocused(bool show);
	void ToggleShowFocused();

	bool GetShowFocused();

	void LoadScene(std::string name);
	void UnloadCurrentScene();
	std::string GetLoadedSceneName();
	Scene* GetLoadedScene();

	void AddScene(std::string name, Scene* element);
	void RemoveScene(std::string name);
	Scene* GetScene(std::string name);
	size_t GetNoOfScenes();
};