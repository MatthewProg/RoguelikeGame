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
	std::string GetText() const;
	const sf::Font* GetFont() const;
	uint32_t GetCharacterSize() const;
	float GetLineSpacing() const;
	float GetLetterSpacing() const;
	uint32_t GetStyle() const;
	const sf::Color& GetFillColor() const;
	const sf::Color& GetOutlineColor() const;
	float GetOutlineThickness() const;
	sf::Vector2f FindCharacterPos(size_t index) const;

	//Text setters
	void SetText(const std::string& string);
	void SetFont(const sf::Font& font);
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
	virtual void ProcessEvent(sf::Event*, const sf::Vector2f&) override;
	virtual sf::FloatRect GetGlobalBounds() const override;
};

