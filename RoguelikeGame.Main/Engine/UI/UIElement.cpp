#include "UIElement.h"

UIElement::UIElement()
{
	_noTexture = Utilities::GetInstance()->NoTexture16x16();
	_texturesManager = nullptr;
	_soundsManager = nullptr;
	_isVisible = true;
	_mouseInput = true;
	_keyboardInput = true;
	_focusOnHover = false;
	_inFocus = false;
}

UIElement::UIElement(UIElement& other)
{
	auto size = other._render.getSize();
	_render.create(size.x, size.y);
	_isVisible = other._isVisible;
	_keyboardInput = other._keyboardInput;
	_mouseInput = other._mouseInput;
	_focusOnHover = other._focusOnHover;
	_inFocus = other._inFocus;
	_noTexture = other._noTexture;
	_texturesManager = other._texturesManager;
	_soundsManager = other._soundsManager;
	setPosition(other.getPosition());
	setRotation(other.getRotation());
	setOrigin(other.getOrigin());
	setScale(other.getScale());
}

void UIElement::Init(sf::Vector2u size)
{
	_render.create(size.x, size.y);
	_render.clear(sf::Color::Transparent);
}

void UIElement::SetVisibility(bool visible)
{
	_isVisible = visible;
}

void UIElement::SetMouseInput(bool active)
{
	_mouseInput = active;
}

void UIElement::SetKeyboardInput(bool active)
{
	_keyboardInput = active;
}

void UIElement::SetTexturesManager(TexturesManager* manager)
{
	_texturesManager = manager;
}

void UIElement::SetSoundsManager(SoundsManager* manager)
{
	_soundsManager = manager;
}

void UIElement::SetFocusOnHover(bool hover)
{
	_focusOnHover = hover;
}

void UIElement::SetInFocus(bool isFocused)
{
	_inFocus = isFocused;
}

bool UIElement::GetVisibility()
{
	return _isVisible;
}

bool UIElement::GetMouseInput()
{
	return _mouseInput;
}

bool UIElement::GetKeyboardInput()
{
	return _keyboardInput;
}

bool UIElement::GetFocusOnHover()
{
	return _focusOnHover;
}

bool UIElement::GetInFocus()
{
	return _inFocus;
}

sf::FloatRect UIElement::GetGlobalBounds()
{
	auto scale = getScale();
	sf::Vector2f size((float)_render.getSize().x, (float)_render.getSize().y);
	sf::Vector2f output(size.x * scale.x, size.y * scale.y);
	return sf::FloatRect(getPosition(), output);
}

sf::RenderTexture* UIElement::GetTexture()
{
	return &_render;
}
