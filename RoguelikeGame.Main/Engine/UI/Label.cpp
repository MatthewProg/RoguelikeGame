#include "Label.h"

Label::Label()
{
	_sthChanged = true;
	_text.setString("");
}

Label::~Label()
{
}

std::string Label::GetText()
{
	return _text.getString();
}

const sf::Font* Label::GetFont()
{
	return _text.getFont();
}

uint32_t Label::GetCharacterSize()
{
	return _text.getCharacterSize();
}

void Label::Update(bool tick, float delta)
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

	sf::RenderStates rs;
	//rs.transform = getTransform();
	_render.draw(_text, rs);

	_render.display();
}

void Label::ProcessEvent(sf::Event* ev, sf::Vector2f mousePos)
{
	//Add TextBox mechanics at some point
}

float Label::GetLineSpacing()
{
	return _text.getLineSpacing();
}

float Label::GetLetterSpacing()
{
	return _text.getLetterSpacing();
}

uint32_t Label::GetStyle()
{
	return _text.getStyle();
}

const sf::Color& Label::GetFillColor()
{
	return _text.getFillColor();
}

const sf::Color& Label::GetOutlineColor()
{
	return _text.getOutlineColor();
}

float Label::GetOutlineThickness()
{
	return _text.getOutlineThickness();
}

sf::Vector2f Label::FindCharacterPos(size_t index)
{
	return _text.findCharacterPos(index);
}

void Label::SetText(std::string string)
{
	_text.setString(string);
	_sthChanged = true;
}

void Label::SetFont(sf::Font* font)
{
	_text.setFont(*font);
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
