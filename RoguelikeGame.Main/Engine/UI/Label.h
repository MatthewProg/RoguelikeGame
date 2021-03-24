#pragma once

#include "../UI/UIElement.h"

#include "SFML/Graphics/Text.hpp"

class Label : public UIElement
{
private:
	sf::Text _text;

	bool _sthChanged;
public:
	Label();
	Label(Label& other);
	~Label();

	//Text getters
	std::string GetText();
	const sf::Font* GetFont();
	uint32_t GetCharacterSize();
	float GetLineSpacing();
	float GetLetterSpacing();
	uint32_t GetStyle();
	const sf::Color& GetFillColor();
	const sf::Color& GetOutlineColor();
	float GetOutlineThickness();
	sf::Vector2f FindCharacterPos(size_t index);

	//Text setters
	void SetText(std::string string);
	void SetFont(sf::Font* font);
	void SetCharacterSize(uint32_t size);
	void SetLineSpacing(float spacing);
	void SetLetterSpacing(float spacing);
	void SetStyle(uint32_t style);
	void SetFillColor(const sf::Color& color);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineThickness(float thickness);

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool, float) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event*, sf::Vector2f) override;
	virtual sf::FloatRect GetGlobalBounds() override;
};

