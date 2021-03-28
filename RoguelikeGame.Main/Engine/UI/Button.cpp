#include "Button.h"

void Button::Update(bool, float)
{

}

void Button::RedrawElement()
{
	_render.clear(sf::Color::Transparent);

	std::string useState = "";
	if (_forcedState != "") useState = _forcedState;
	else useState = _currentState;

	sf::VertexArray bg(sf::PrimitiveType::Quads, 4);
	sf::RenderStates rs;

	auto text = _textStates.find(useState);
	auto back = _backgroundStates.find(useState);

	bg[0].position = sf::Vector2f(0.f, 0.f);
	bg[1].position = sf::Vector2f(_backgroundSize.x, 0.f);
	bg[2].position = sf::Vector2f(_backgroundSize.x, _backgroundSize.y);
	bg[3].position = sf::Vector2f(0.f, _backgroundSize.y);

	sf::Texture* texture = nullptr;
	if(back != _backgroundStates.end())
		texture = _texturesManager->GetTexture(std::get<0>(back->second));
	if (back != _backgroundStates.end() && texture != nullptr && texture->getSize().x > 0)
	{
		auto &rect = std::get<1>(back->second);
		bg[0].texCoords = sf::Vector2f(rect.left, rect.top);
		bg[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
		bg[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		bg[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);

		rs.texture = texture;
	}
	else
	{
		bg[0].texCoords = sf::Vector2f(0, 0);
		bg[1].texCoords = sf::Vector2f(16, 0);
		bg[2].texCoords = sf::Vector2f(16, 16);
		bg[3].texCoords = sf::Vector2f(0, 16);

		rs.texture = _noTexture;
	}

	_render.draw(bg, rs);

	if (text != _textStates.end())
	{
		rs.texture = nullptr;
		_render.draw(text->second, rs);
	}

	_render.display();
}

void Button::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	std::string newState = "none";
	if (_mouseInput == true)
	{
		_lmbUp = false;
		auto bounds = GetGlobalBounds();
		if (ev->type == sf::Event::MouseMoved)
			if (bounds.contains(mousePos))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					newState = "click";
				else
					newState = "hover";
			}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (bounds.contains(mousePos))
			{
				_lmbWasDown = true;
				newState = "click";
			}
		if (ev->type == sf::Event::MouseButtonReleased && ev->mouseButton.button == sf::Mouse::Left)
		{
			if (bounds.contains(mousePos) && _lmbWasDown == true)
			{
				_lmbUp = true;
				_lmbWasDown = false;
				newState = "none";
			}
			else if (bounds.contains(mousePos) && _lmbWasDown == false)
			{
				_lmbUp = false;
				newState = "hover";
			}
			else
				_lmbUp = false;
		}
	}
	if (_keyboardInput == true)
	{
		if (ev->type == sf::Event::KeyPressed && ev->key.code == sf::Keyboard::Enter)
		{
			if (_inFocus == true)
				newState = "click";
			else
				newState = "none";
		}
		if (ev->type == sf::Event::KeyReleased && ev->key.code == sf::Keyboard::Enter)
		{
			if (_inFocus == true)
				_lmbUp = true;
			newState = "none";
		}
	}

	if (newState != _currentState)
	{
		if (_forcedState == "")
		{
			if (_soundsManager != nullptr)
			{
				sf::Sound* sound = nullptr;
				if (_currentState == "none" && newState == "hover") sound = _soundsManager->GetSound("ui_select");
				else if (newState == "click") sound = _soundsManager->GetSound("ui_click");
				else if (_currentState == "click") sound = _soundsManager->GetSound("ui_release");

				if (sound != nullptr) sound->play();
			}

			_currentState = newState;
			RedrawElement();
		}
	}
}

UIElement* Button::clone() 
{
	return new Button(*this);
}

Button::Button()
{
	_textStates.clear();
	_backgroundStates.clear();
	_forcedState = "";
	_currentState = "none";
	_lmbWasDown = false;
	_lmbUp = false;
	_keyboardInput = false;
	_mouseInput = true;
}

Button::Button(Button& other) : UIElement(other)
{
	_textStates = other._textStates;
	_backgroundStates = other._backgroundStates;
	_forcedState = other._forcedState;
	_currentState = other._currentState;
	_backgroundSize = other._backgroundSize;
	_lmbWasDown = other._lmbWasDown;
	_lmbUp = other._lmbUp;
}

Button::~Button()
{
}

void Button::ForceState(const std::string& state)
{
	_forcedState = state;
	RedrawElement();
}

void Button::ResetForcedState()
{
	_forcedState = "";
	RedrawElement();
}

const std::string& Button::GetCurrentState() const
{
	if (_forcedState != "") return _forcedState;
	else return _currentState;
}

bool Button::IsForcingState() const
{
	return (_forcedState != "");
}

void Button::AddState(const std::string& name, const sf::Text& textState, const std::string& textureName, const sf::FloatRect& backgroundRect)
{
	_textStates[name] = textState;
	_backgroundStates[name] = std::tuple<std::string, sf::FloatRect>(textureName, backgroundRect);
	RedrawElement();
}

void Button::RemoveState(const std::string& name)
{
	auto found = _textStates.find(name);
	if (found != _textStates.end())
		_textStates.erase(found);

	auto found2 = _backgroundStates.find(name);
	if (found2 != _backgroundStates.end())
		_backgroundStates.erase(found2);

	if (_forcedState == name) _forcedState = "";
	if (_currentState == name) _currentState = "none";
}

sf::Text* Button::EditTextState(const std::string& name)
{
	auto found = _textStates.find(name);
	if (found != _textStates.end())
		return &found->second;
	return nullptr;
}

std::tuple<std::string, sf::FloatRect>* Button::EditBackgroundState(const std::string& name)
{
	auto found = _backgroundStates.find(name);
	if (found != _backgroundStates.end())
		return &found->second;
	return nullptr;
}

bool Button::Clicked() const
{
	return _lmbUp;
}

void Button::SetBackgroundSize(const sf::Vector2f& size)
{
	_backgroundSize = size;
	RedrawElement();
}

void Button::ApplyText(const std::string& string)
{
	for (auto& t : _textStates)
		t.second.setString(string);
}

void Button::ApplyFont(const sf::Font* font)
{
	for (auto& t : _textStates)
		t.second.setFont(*font);
}

void Button::ApplyCharacterSize(uint32_t size)
{
	for (auto& t : _textStates)
		t.second.setCharacterSize(size);
}

void Button::ApplyLineSpacing(float spacing)
{
	for (auto& t : _textStates)
		t.second.setLineSpacing(spacing);
}

void Button::ApplyLetterSpacing(float spacing)
{
	for (auto& t : _textStates)
		t.second.setLetterSpacing(spacing);
}

void Button::ApplyStyle(uint32_t style)
{
	for (auto& t : _textStates)
		t.second.setStyle(style);
}

void Button::ApplyFillColor(const sf::Color& color)
{
	for (auto& t : _textStates)
		t.second.setFillColor(color);
}

void Button::ApplyOutlineColor(const sf::Color& color)
{
	for (auto& t : _textStates)
		t.second.setOutlineColor(color);
}

void Button::ApplyOutlineThickness(float thickness)
{
	for (auto& t : _textStates)
		t.second.setOutlineThickness(thickness);
}

const sf::Vector2f& Button::GetBackgroundSize() const
{
	return _backgroundSize;
}
