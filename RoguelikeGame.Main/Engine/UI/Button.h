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
	~Button();

	void ForceState(std::string state);
	void ResetForcedState();
	std::string GetCurrentState();
	bool IsForcingState();

	void AddState(std::string name, sf::Text textState, std::string textureName, sf::FloatRect backgroundRect);
	void RemoveState(std::string name);

	bool Clicked();

	void SetBackgroundSize(sf::Vector2f size);

	sf::Vector2f GetBackgroundSize();

	// Inherited via UIElement
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, sf::Vector2f mousePos) override;
};

