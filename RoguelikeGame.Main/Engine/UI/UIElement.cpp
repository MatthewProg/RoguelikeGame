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

void UIElement::Init(const sf::Vector2u& size)
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
	if (_mouseInput == false && _keyboardInput == false)
		_inFocus = false;
}

bool UIElement::GetVisibility() const
{
	return _isVisible;
}

bool UIElement::GetMouseInput() const
{
	return _mouseInput;
}

bool UIElement::GetKeyboardInput() const
{
	return _keyboardInput;
}

bool UIElement::GetFocusOnHover() const
{
	return _focusOnHover;
}

bool UIElement::GetInFocus() const
{
	return _inFocus;
}

sf::FloatRect UIElement::GetGlobalBounds() const
{
	auto &scale = getScale();
	sf::Vector2f size((float)_render.getSize().x, (float)_render.getSize().y);
	sf::Vector2f output(size.x * scale.x, size.y * scale.y);
	return sf::FloatRect(getPosition(), output);
}

std::vector<sf::Vector2f> UIElement::GetAllBoundsPoints() const
{
	return CollisionHelper::GetRectPoints(GetGlobalBounds());
}

std::vector<sf::Vector2f> UIElement::GetDeepestInFocusBoundsPoints() const
{
	if (_inFocus)
		return UIElement::GetAllBoundsPoints();
	std::vector<sf::Vector2f> output{ sf::Vector2f(-1.f,-1.f), sf::Vector2f(-1.f,-1.f), sf::Vector2f(-1.f,-1.f), sf::Vector2f(-1.f,-1.f) };
	return output;
}

const sf::RenderTexture* UIElement::GetTexture() const
{
	return &_render;
}
