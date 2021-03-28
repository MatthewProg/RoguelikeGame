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
	void UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos);
	void Update(bool tick, float delta);
	void RefreshElements();

	void AddElement(const std::string& name, UIElement* element);
	void RemoveElement(UIElement* element);
	void RemoveElement(const std::string& name);

	//Setters
	void SetBackgroundColor(const sf::Color& color);
	void SetElementVisibility(const std::string& name, bool visible);
	void SetFocus(const std::string& name);
	void ClearFocus();

	//Getters
	const sf::Color& GetBackgroundColor() const;
	bool GetElementVisibility(const std::string& name) const;
	UIElement* GetElement(const std::string& name);
	const std::tuple<std::string, UIElement*>& GetFocused() const;
	size_t GetNoOfElements() const;
};