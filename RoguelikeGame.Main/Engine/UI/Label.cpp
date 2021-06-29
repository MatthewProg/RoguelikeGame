#include "Label.h"

void Label::RefreshAlignment()
{
	//Vertically
	if (_verticalAlignment == UIElement::Align::START)
	{
		auto pos = _text.getPosition();
		pos.y = 0.f;
		_text.setPosition(pos);
	}
	else if (_verticalAlignment == UIElement::Align::MIDDLE)
	{
		auto &pos = _text.getPosition();
		auto newPos = ViewHelper::GetScaled(sf::FloatRect(0.f, 0.5f, 1.f, 1.f), _text.getGlobalBounds(), GetGlobalBounds());
		_text.setPosition(pos.x, newPos.top - getPosition().y);
	}
	else if (_verticalAlignment == UIElement::Align::END)
	{
		auto bounds = _text.getGlobalBounds();
		auto box = GetGlobalBounds();
		_text.setPosition(bounds.left, box.height - bounds.height);
	}


	//Horizontally
	if (_horizontalAlignment == UIElement::Align::START)
	{
		auto pos = _text.getPosition();
		pos.x = 0.f;
		_text.setPosition(pos);
	}
	else if (_horizontalAlignment == UIElement::Align::MIDDLE)
	{
		auto &pos = _text.getPosition();
		auto newPos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.f, 1.f, 1.f), _text.getGlobalBounds(), GetGlobalBounds());
		_text.setPosition(newPos.left - getPosition().x, pos.y);
	}
	else if(_horizontalAlignment == UIElement::Align::END)
	{
		auto bounds = _text.getGlobalBounds();
		auto box = GetGlobalBounds();
		_text.setPosition(box.width - bounds.width, bounds.top);
	}
}

Label::Label()
{
	_sthChanged = true;
	_text.setString("");
	_mouseInput = false;
	_keyboardInput = false;
	_verticalAlignment = Align::START;
	_horizontalAlignment = Align::START;
}

Label::Label(Label& other) : UIElement(other)
{
	_text = other._text;
	_sthChanged = other._sthChanged;
	_verticalAlignment = other._verticalAlignment;
	_horizontalAlignment = other._horizontalAlignment;
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
	RefreshAlignment();
	_render.clear(sf::Color::Transparent);
	_render.draw(_text);
	_render.display();
}

void Label::ProcessEvent(sf::Event*, const sf::Vector2f&)
{
	//Add TextBox mechanics at some point
}

//sf::FloatRect Label::GetGlobalBounds() const
//{
//	return getTransform().transformRect(_text.getGlobalBounds());
//}

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

sf::FloatRect Label::GetTextGlobalBounds() const
{
	return getTransform().transformRect(_text.getGlobalBounds());
}

UIElement::Align Label::GetVerticalAlignment() const
{
	return _verticalAlignment;
}

UIElement::Align Label::GetHorizontalAlignment() const
{
	return _horizontalAlignment;
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

void Label::SetVerticalAlignment(UIElement::Align align)
{
	if (align != _verticalAlignment)
	{
		_verticalAlignment = align;
		_sthChanged = true;
	}
}

void Label::SetHorizontalAlignment(UIElement::Align align)
{
	if (align != _horizontalAlignment)
	{
		_horizontalAlignment = align;
		_sthChanged = true;
	}
}
