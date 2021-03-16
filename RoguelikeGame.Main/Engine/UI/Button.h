#pragma once

#include "../UI/UIElement.h"

#include "SFML/Graphics/Text.hpp"

class Button : public UIElement
{
private:
	std::map<std::string, sf::Text> _textStates;
	std::map<std::string, std::tuple<std::string, sf::FloatRect>> _backgroundStates;

	std::string _forcedState;
	std::string _currentState;

	sf::Vector2f _backgroundSize;

	bool _lmbWasDown;
	bool _lmbUp;
public:
	Button();
	Button(Button& other);
	~Button();

	void ForceState(std::string state);
	void ResetForcedState();
	std::string GetCurrentState();
	bool IsForcingState();

	void AddState(std::string name, sf::Text textState, std::string textureName, sf::FloatRect backgroundRect);
	void RemoveState(std::string name);
	sf::Text* EditTextState(std::string name);
	std::tuple<std::string, sf::FloatRect>* EditBackgroundState(std::string name);

	bool Clicked();

	void SetBackgroundSize(sf::Vector2f size);
	void ApplyText(std::string string);
	void ApplyFont(sf::Font* font);
	void ApplyCharacterSize(uint32_t size);
	void ApplyLineSpacing(float spacing);
	void ApplyLetterSpacing(float spacing);
	void ApplyStyle(uint32_t style);
	void ApplyFillColor(const sf::Color& color);
	void ApplyOutlineColor(const sf::Color& color);
	void ApplyOutlineThickness(float thickness);

	sf::Vector2f GetBackgroundSize();

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, sf::Vector2f mousePos) override;
};

