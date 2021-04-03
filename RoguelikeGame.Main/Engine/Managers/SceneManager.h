#pragma once

#include <map>

#include "../UI/Scene.h"
#include "../Helpers/CollisionHelper.h"

#include "SFML/Graphics/Drawable.hpp"

class SceneManager : public sf::Drawable 
{
private:
	Logger* _logger;
	std::map<std::string, Scene*> _scenes;
	
	std::string _loadedScene;

	sf::VertexArray _focusedOutline;
	sf::VertexArray _allOutline;
	bool _showFocused;
	bool _showAllBounds;

	void UpdateAllBoundsOutline();

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	SceneManager();
	~SceneManager();

	void UpdateFocus(const sf::Vector2f& mousePos, bool clicked);
	void UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos);
	void Update(bool tick, float delta);

	void SetShowFocused(bool show);
	void SetShowAllBounds(bool show);
	void ToggleShowFocused();

	bool GetShowFocused() const;

	void LoadScene(const std::string& name);
	void UnloadCurrentScene();
	const std::string& GetLoadedSceneName() const;
	Scene* GetLoadedScene();

	void AddScene(const std::string& name, Scene* element);
	void RemoveScene(const std::string& name);
	Scene* GetScene(const std::string& name);
	size_t GetNoOfScenes() const;
};