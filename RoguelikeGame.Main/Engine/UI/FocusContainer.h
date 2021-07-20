#pragma once

#include "../UI/Container.h"

class FocusContainer : public Container
{
private:
	bool _passFocus;
	bool _passClick;
	bool _enabled;

	sf::Color _hoverColor;
	sf::Color _focusColor;
	sf::VertexArray _background;

	void ChangeColor(const sf::Color& col);
public:
	FocusContainer();
	FocusContainer(FocusContainer& other);
	~FocusContainer() = default;

	//Setters
	void SetPassHover(bool pass);
	void SetPassClick(bool pass);
	void SetHoverColor(const sf::Color& color);
	void SetFocusColor(const sf::Color& color);
	void SetEnabled(bool enabled);

	//Getters
	bool GetPassFocus() const;
	bool GetPassClick() const;
	const sf::Color& GetHoverColor() const;
	const sf::Color& GetFocusColor() const;
	bool IsEnabled() const;

	// Inherited via UIElement
	UIElement* clone() override;
	void Update(bool tick, float delta) override;
	void ForceRedraw() override;
	bool Redraw() override;
	void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
};

