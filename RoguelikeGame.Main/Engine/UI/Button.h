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

	void ForceState(const std::string& state);
	void ResetForcedState();
	const std::string& GetCurrentState() const;
	bool IsForcingState() const;

	void AddState(const std::string& name, const sf::Text& textState, const std::string& textureName, const sf::FloatRect& backgroundRect);
	void RemoveState(const std::string& name);
	sf::Text* EditTextState(const std::string& name);
	std::tuple<std::string, sf::FloatRect>* EditBackgroundState(const std::string& name);

	bool Clicked() const;

	void SetBackgroundSize(const sf::Vector2f& size);
	void ApplyText(const std::string& string);
	void ApplyFont(const sf::Font* font);
	void ApplyCharacterSize(uint32_t size);
	void ApplyLineSpacing(float spacing);
	void ApplyLetterSpacing(float spacing);
	void ApplyStyle(uint32_t style);
	void ApplyFillColor(const sf::Color& color);
	void ApplyOutlineColor(const sf::Color& color);
	void ApplyOutlineThickness(float thickness);

	const sf::Vector2f& GetBackgroundSize() const;

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool, float) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
};

