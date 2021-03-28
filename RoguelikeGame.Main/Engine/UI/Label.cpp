#include "Label.h"

Label::Label()
{
	_sthChanged = true;
	_text.setString("");
	_mouseInput = false;
	_keyboardInput = false;
}

Label::Label(Label& other) : UIElement(other)
{
	_text = other._text;
	_sthChanged = other._sthChanged;
}

Label::~Label()
{
}

std::string Label::GetText() const
{
	return _text.getString();
}

const sf::Font* Label::GetFont() const
{
	return _text.getFont();
}

uint32_t Label::GetCharacterSize() const
{
	return _text.getCharacterSize();
}

void Label::Update(bool, float)
{
	if (_sthChanged)
	{
		RedrawElement();
		_sthChanged = false;
	}
}

void Label::RedrawElement()
{
	_render.clear(sf::Color::Transparent);
	_render.draw(_text);
	_render.display();
}

void Label::ProcessEvent(sf::Event*, const sf::Vector2f&)
{
	//Add TextBox mechanics at some point
}

sf::FloatRect Label::GetGlobalBounds() const
{
	return getTransform().transformRect(_text.getGlobalBounds());
}

UIElement* Label::clone()
{
	return new Label(*this);
}

float Label::GetLineSpacing() const
{
	return _text.getLineSpacing();
}

float Label::GetLetterSpacing() const
{
	return _text.getLetterSpacing();
}

uint32_t Label::GetStyle() const
{
	return _text.getStyle();
}

const sf::Color& Label::GetFillColor() const
{
	return _text.getFillColor();
}

const sf::Color& Label::GetOutlineColor() const
{
	return _text.getOutlineColor();
}

float Label::GetOutlineThickness() const
{
	return _text.getOutlineThickness();
}

sf::Vector2f Label::FindCharacterPos(size_t index) const
{
	return _text.findCharacterPos(index);
}

void Label::SetText(const std::string& string)
{
	_text.setString(string);
	_sthChanged = true;
}

void Label::SetFont(const sf::Font& font)
{
	_text.setFont(font);
	_sthChanged = true;
}

void Label::SetCharacterSize(uint32_t size)
{
	_text.setCharacterSize(size);
	_sthChanged = true;
}

void Label::SetLineSpacing(float spacing)
{
	_text.setLineSpacing(spacing);
	_sthChanged = true;
}

void Label::SetLetterSpacing(float spacing)
{
	_text.setLetterSpacing(spacing);
	_sthChanged = true;
}

void Label::SetStyle(uint32_t style)
{
	_text.setStyle(style);
	_sthChanged = true;
}

void Label::SetFillColor(const sf::Color& color)
{
	_text.setFillColor(color);
	_sthChanged = true;
}

void Label::SetOutlineColor(const sf::Color& color)
{
	_text.setOutlineColor(color);
	_sthChanged = true;
}

void Label::SetOutlineThickness(float thickness)
{
	_text.setOutlineThickness(thickness);
	_sthChanged = true;
}
