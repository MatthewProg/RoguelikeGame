#pragma once

#include "../UI/UIElement.h"

class CheckBox : public UIElement
{
private:
	bool _isChecked;

	sf::FloatRect _checkedRect;
	sf::FloatRect _uncheckedRect;
	std::string _textureName;
public:
	CheckBox();
	CheckBox(CheckBox& other);
	~CheckBox() override = default;

	void SetChecked(bool check);
	bool IsChecked() const;

	void SetCheckedRect(const sf::FloatRect& rect);
	void SetUncheckedRect(const sf::FloatRect& rect);
	void SetTextureName(const std::string& name);

	const sf::FloatRect& GetCheckedRect() const;
	const sf::FloatRect& GetUncheckedRect() const;
	const std::string& GetTextureName() const;


	// Inherited via UIElement
	UIElement* clone() override;
	void Update(bool, float) override;
	void ForceRedraw() override;
	void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
};

