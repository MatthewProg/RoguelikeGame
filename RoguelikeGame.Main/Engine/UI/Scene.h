#pragma once

#include <map>

#include "../UI/UIElement.h"

#include "SFML/Graphics/Drawable.hpp"

class Scene : public sf::Drawable 
{
private:
	std::map<std::string, UIElement*> _uiElements;
	sf::Color _backgroundColor;

	std::tuple<std::string, UIElement*> _inFocus;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	Scene();
	Scene(Scene& other);
	~Scene();

	void UpdateFocus(const sf::Vector2f& mousePos, bool clicked);
	void UpdateEvent(sf::Event* ev, sf::Vector2f mousePos);
	void Update(bool tick, float delta);
	void RefreshElements();

	void AddElement(std::string name, UIElement* element);
	void RemoveElement(UIElement* element);
	void RemoveElement(std::string name);

	//Setters
	void SetBackgroundColor(sf::Color color);
	void SetElementVisibility(std::string name, bool visible);
	void SetFocus(std::string name);
	void ClearFocus();

	//Getters
	sf::Color GetBackgroundColor();
	bool GetElementVisibility(std::string name);
	UIElement* GetElement(std::string name);
	std::tuple<std::string, UIElement*> GetFocused();
	size_t GetNoOfElements();
};