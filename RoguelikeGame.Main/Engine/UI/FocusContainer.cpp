#include "FocusContainer.h"

UIElement* FocusContainer::clone()
{
	return new FocusContainer(*this);
}

void FocusContainer::Update(bool tick, float delta)
{
	for (auto& e : _uiElements)
		if (e.second != nullptr)
			e.second->Update(tick, delta);

	if (_sthChanged)
	{
		RedrawElement();
		_sthChanged = false;
	}
}

void FocusContainer::RedrawElement()
{
	_render.clear(sf::Color::Transparent);
	_render.draw(_background);
	sf::RenderStates rs;
	sf::VertexArray draw(sf::Quads, 4);

	for (auto& e : _uiElements)
		if (e.second != nullptr)
		{
			e.second->RedrawElement();

			for (auto& element : _uiElements)
			{
				if (element.second->GetVisibility() == false) continue;
				auto texture = element.second->GetTexture();
				auto size = texture->getSize();
				rs.texture = &texture->getTexture();
				rs.transform = element.second->getTransform();
				draw[0].position = sf::Vector2f(0, 0);
				draw[1].position = sf::Vector2f((float)size.x, 0);
				draw[2].position = sf::Vector2f((float)size.x, (float)size.y);
				draw[3].position = sf::Vector2f(0, (float)size.y);
				draw[0].texCoords = sf::Vector2f(0, 0);
				draw[1].texCoords = sf::Vector2f((float)size.x, 0);
				draw[2].texCoords = sf::Vector2f((float)size.x, (float)size.y);
				draw[3].texCoords = sf::Vector2f(0, (float)size.y);
				_render.draw(draw, rs);
			}
		}
	_render.display();
}

void FocusContainer::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	bool contains = false;
	if (GetGlobalBounds().contains(mousePos)) contains = true;

	if (ev->type == sf::Event::MouseButtonPressed)
	{
		if (contains == true)
		{
			ChangeColor(_focusColor);
			SetInFocus(true);
		}
		else
		{
			ChangeColor(sf::Color::Transparent);
			SetInFocus(false);
		}
	}
	else if (contains == true && ev->type == sf::Event::MouseMoved && GetInFocus() == false)
		ChangeColor(_hoverColor);
	else if (GetInFocus() == false)
		ChangeColor(sf::Color::Transparent);

	for (auto& e : _uiElements)
	{
		if (e.second == nullptr) continue;

		if (_passFocus) e.second->SetInFocus(_inFocus);
		if (_passClick == true && contains == true && (ev->type == sf::Event::MouseButtonPressed || ev->type == sf::Event::MouseButtonReleased))
		{
			auto b = e.second->GetGlobalBounds();
			if (b.contains(mousePos - getPosition()))
				e.second->ProcessEvent(ev, mousePos - getPosition());
			else
			{
				sf::Vector2f newPos = ViewHelper::GetRectCenter(b);
				e.second->ProcessEvent(ev, newPos);
			}
		}
		else
			e.second->ProcessEvent(ev, mousePos - getPosition());
	}

	if (ev->type == sf::Event::MouseButtonPressed || ev->type == sf::Event::MouseButtonReleased ||
		ev->type == sf::Event::KeyPressed || ev->type == sf::Event::KeyReleased || ev->type == sf::Event::MouseMoved)
		_sthChanged = true;
}

void FocusContainer::ChangeColor(const sf::Color& col)
{
	if (col != _background[0].color)
	{
		for (size_t i = 0; i < _background.getVertexCount(); i++)
			_background[i].color = col;

		auto b = GetGlobalBounds();
		_background[0].position = sf::Vector2f(0.f, 0.f);
		_background[1].position = sf::Vector2f(b.width, 0.f);
		_background[2].position = sf::Vector2f(b.width, b.height);
		_background[3].position = sf::Vector2f(0.f, b.height);
		_sthChanged = true;
	}
}

FocusContainer::FocusContainer()
{
	_uiElements.clear();
	_passFocus = true;
	_passClick = true;
	_sthChanged = true;
	_hoverColor = sf::Color(0, 0, 0, 32);
	_focusColor = sf::Color(0, 0, 0, 64);
	_background.setPrimitiveType(sf::PrimitiveType::Quads);
	_background.resize(4);
	_isVisible = true;
	_keyboardInput = true;
	_mouseInput = true;
	_focusOnHover = false;
}

FocusContainer::FocusContainer(FocusContainer& other) : UIElement(other)
{
	for (auto& e : other._uiElements)
		_uiElements[e.first] = e.second->clone();
	_passFocus = other._passFocus;
	_passClick = other._passClick;
	_hoverColor = other._hoverColor;
	_focusColor = other._focusColor;
	_background = other._background;
	_sthChanged = other._sthChanged;
}

FocusContainer::~FocusContainer()
{
	for (auto& p : _uiElements)
		if (p.second != nullptr)
			delete p.second;
}

void FocusContainer::AddElement(const std::string& name, UIElement* element)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
	}

	_uiElements[name] = element;
}

void FocusContainer::RemoveElement(UIElement* element)
{
	for (auto& p : _uiElements)
		if (p.second == element)
		{
			if (p.second != nullptr)
				delete p.second;
			_uiElements.erase(p.first);
		}
}

void FocusContainer::RemoveElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
	}
}

UIElement* FocusContainer::GetElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second;
	else
		return nullptr;
}

const std::map<std::string, UIElement*>& FocusContainer::GetElements() const
{
	return _uiElements;
}

void FocusContainer::SetPassHover(bool pass)
{
	_passFocus = pass;
}

void FocusContainer::SetPassClick(bool pass)
{
	_passClick = pass;
}

void FocusContainer::SetHoverColor(const sf::Color& color)
{
	_hoverColor = color;
}

void FocusContainer::SetFocusColor(const sf::Color& color)
{
	_focusColor = color;
}

bool FocusContainer::GetPassFocus() const
{
	return _passFocus;
}

bool FocusContainer::GetPassClick() const
{
	return _passClick;
}

const sf::Color& FocusContainer::GetHoverColor() const
{
	return _hoverColor;
}

const sf::Color& FocusContainer::GetFocusColor() const
{
	return _focusColor;
}

sf::FloatRect FocusContainer::GetElementsGlobalBounds() const
{
	sf::Vector2f size;
	for (auto& e : _uiElements)
	{
		if (e.second == nullptr) continue;

		auto b = e.second->GetGlobalBounds();
		sf::Vector2f rd(b.left + b.width, b.top + b.height);
		if (rd.x > size.x) size.x = rd.x;
		if (rd.y > size.y) size.y = rd.y;
	}

	return sf::FloatRect(getPosition(), size);
}
