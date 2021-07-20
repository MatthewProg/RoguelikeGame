#include "FocusContainer.h"

UIElement* FocusContainer::clone()
{
	return new FocusContainer(*this);
}

void FocusContainer::Update(bool tick, float delta)
{
	Container::Update(tick, delta);
	Redraw();
}

void FocusContainer::ForceRedraw()
{
	Container::ForceRedraw();
	Redraw();
}

bool FocusContainer::Redraw()
{
	for (auto& e : _uiElements)
		if (e.second != nullptr)
			if (e.second->RedrawHappened())
			{
				_sthChanged = true;
				break;
			}

	if (_sthChanged)
	{
		_render.clear(sf::Color::Transparent);
		_render.draw(_background);
		sf::RenderStates rs;
		sf::VertexArray draw(sf::Quads, 4);

		for (auto& e : _uiElements)
		{
			if (e.second != nullptr)
			{
				if (e.second->GetVisibility() == false) continue;
				auto texture = e.second->GetTexture();
				auto size = texture->getSize();
				rs.texture = &texture->getTexture();
				rs.transform = e.second->getTransform();
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

		if (_enabled == false)
		{
			for (size_t i = 0; i < _background.getVertexCount(); i++)
				draw[i].color = sf::Color(0, 0, 0, 128);

			auto b = GetGlobalBounds();
			draw[0].position = sf::Vector2f(0.f, 0.f);
			draw[1].position = sf::Vector2f(b.width, 0.f);
			draw[2].position = sf::Vector2f(b.width, b.height);
			draw[3].position = sf::Vector2f(0.f, b.height);
			_render.draw(draw);
		}
		_render.display();

		_sthChanged = false;
		_redrawHappened = true;
		return true;
	}
	return false;
}

void FocusContainer::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	if (_enabled == false)
		return;

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
	else if (GetInFocus() == true)
		ChangeColor(_focusColor);

	auto transformedMouse = getInverseTransform().transformPoint(mousePos);
	for (auto& e : _uiElements)
	{
		if (e.second == nullptr) continue;

		if (_passFocus) e.second->SetInFocus(_inFocus);
		if (_passClick == true && contains == true && (ev->type == sf::Event::MouseButtonPressed || ev->type == sf::Event::MouseButtonReleased))
		{
			auto b = e.second->GetGlobalBounds();
			if (b.contains(transformedMouse))
				e.second->ProcessEvent(ev, transformedMouse);
			else
			{
				sf::Vector2f newPos = ViewHelper::GetRectCenter(b);
				e.second->ProcessEvent(ev, newPos);
			}
		}
		else
			e.second->ProcessEvent(ev, transformedMouse);
	}
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
	_passFocus = true;
	_passClick = true;
	_hoverColor = sf::Color(0, 0, 0, 32);
	_focusColor = sf::Color(0, 0, 0, 64);
	_background.setPrimitiveType(sf::PrimitiveType::Quads);
	_background.resize(4);
	_isVisible = true;
	_keyboardInput = true;
	_mouseInput = true;
	_focusOnHover = false;
	_enabled = true;
}

FocusContainer::FocusContainer(FocusContainer& other) : Container(other)
{
	_passFocus = other._passFocus;
	_passClick = other._passClick;
	_hoverColor = other._hoverColor;
	_focusColor = other._focusColor;
	_background = other._background;
	_enabled = other._enabled;
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

void FocusContainer::SetEnabled(bool enabled)
{
	if (enabled != _enabled)
	{
		if (enabled == false)
		{
			ChangeColor(sf::Color::Transparent);
			SetInFocus(false);
			for (auto& e : _uiElements)
			{
				if (e.second == nullptr) continue;
				e.second->SetInFocus(false);
			}
		}
		_enabled = enabled;
		_sthChanged = true;
	}
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

bool FocusContainer::IsEnabled() const
{
	return _enabled;
}
