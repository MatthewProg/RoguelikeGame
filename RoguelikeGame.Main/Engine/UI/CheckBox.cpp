#include "CheckBox.h"

UIElement* CheckBox::clone()
{
	return new CheckBox(*this);
}

void CheckBox::Update(bool, float)
{
	Redraw();
}

void CheckBox::ForceRedraw()
{
	_render.clear(sf::Color::Transparent);

	sf::Texture* tex = _texturesManager->GetTexture(_textureName);
	if (tex == nullptr || tex->getSize().x == 0)
		tex = _noTexture;

	auto rect = (_isChecked == true) ? _checkedRect : _uncheckedRect;
	sf::VertexArray box(sf::PrimitiveType::Quads, 4);
	box[0].position = sf::Vector2f(0.f, 0.f);
	box[1].position = sf::Vector2f(rect.width, 0.f);
	box[2].position = sf::Vector2f(rect.width, rect.height);
	box[3].position = sf::Vector2f(0.f, rect.height);

	auto texRect = rect;
	if (tex == _noTexture)
		texRect = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
	box[0].texCoords = sf::Vector2f(texRect.left, texRect.top);
	box[1].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top);
	box[2].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height);
	box[3].texCoords = sf::Vector2f(texRect.left, texRect.top + texRect.height);
	_render.draw(box, tex);
	_render.display();

	_sthChanged = false;
	_redrawHappened = true;
}

void CheckBox::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	if (_mouseInput)
	{
		if (ev->type == sf::Event::MouseButtonReleased && ev->mouseButton.button == sf::Mouse::Left)
		{
			auto bounds = GetGlobalBounds();
			if (bounds.contains(mousePos))
				SetChecked(!_isChecked);
		}
	}

	if (_keyboardInput)
	{
		if (ev->type == sf::Event::KeyReleased && ev->key.code == sf::Keyboard::Space)
		{
			if(_inFocus)
				SetChecked(!_isChecked);
		}
	}
}

CheckBox::CheckBox()
{
	_isChecked = false;
	_checkedRect = sf::FloatRect(0, 0, 0, 0);
	_uncheckedRect = sf::FloatRect(0, 0, 0, 0);
	_textureName = "";
}

CheckBox::CheckBox(CheckBox& other) : UIElement(other)
{
	_isChecked = other._isChecked;
	_checkedRect = other._checkedRect;
	_uncheckedRect = other._uncheckedRect;
	_textureName = other._textureName;
}

CheckBox::~CheckBox()
{
	;
}

void CheckBox::SetChecked(bool check)
{
	if (check != _isChecked)
	{
		_isChecked = check;
		_sthChanged = true;
	}
}

bool CheckBox::IsChecked() const
{
	return _isChecked;
}

void CheckBox::SetCheckedRect(const sf::FloatRect& rect)
{
	_checkedRect = rect;
	_sthChanged = true;
}

void CheckBox::SetUncheckedRect(const sf::FloatRect& rect)
{
	_uncheckedRect = rect;
	_sthChanged = true;
}

void CheckBox::SetTextureName(const std::string& name)
{
	_textureName = name;
	_sthChanged = true;
}

const sf::FloatRect& CheckBox::GetCheckedRect() const
{
	return _checkedRect;
}

const sf::FloatRect& CheckBox::GetUncheckedRect() const
{
	return _uncheckedRect;
}

const std::string& CheckBox::GetTextureName() const
{
	return _textureName;
}
