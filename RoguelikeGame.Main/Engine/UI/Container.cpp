#include "Container.h"

Container::Container()
{
	_uiElements.clear();
}

Container::Container(Container& other) : UIElement(other)
{
	for (auto& e : other._uiElements)
		_uiElements[e.first] = e.second->clone();
}

Container::~Container()
{
	for (auto& p : _uiElements)
		if (p.second != nullptr)
			delete p.second;
}

void Container::AddElement(const std::string& name, UIElement* element)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
	}

	_uiElements[name] = element;
	_sthChanged = true;
}

void Container::RemoveElement(UIElement* element)
{
	for (auto& p : _uiElements)
		if (p.second == element)
		{
			if (p.second != nullptr)
				delete p.second;
			_uiElements.erase(p.first);
			_sthChanged = true;
		}
}

void Container::RemoveElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
		_sthChanged = true;
	}
}

void Container::RenameElement(const std::string& oldName, const std::string& newName)
{
	auto found = _uiElements.find(oldName);
	if (found != _uiElements.end())
	{
		RemoveElement(newName);
		_uiElements.insert(*found);
		_uiElements.erase(found);
	}
}

UIElement* Container::GetElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second;
	else
		return nullptr;
}

const std::map<std::string, UIElement*>& Container::GetElements() const
{
	return _uiElements;
}

void Container::AutoAlignElementsHorizontally(Align align)
{
	switch (align)
	{
	case UIElement::Align::START:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto pos = e.second->getPosition();
			pos.x = 0.f;
			e.second->setPosition(pos);
		}
		break;
	case UIElement::Align::MIDDLE:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto& pos = e.second->getPosition();
			auto newPos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.f, 1.f, 1.f), e.second->GetGlobalBounds(), GetGlobalBounds());

			e.second->setPosition(newPos.left - getPosition().x, pos.y);
		}
		break;
	case UIElement::Align::END:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto pos = e.second->getPosition();
			auto bounds = e.second->GetGlobalBounds();
			auto size = GetGlobalBounds();

			pos.x = size.width - bounds.width;
			e.second->setPosition(pos);
		}
		break;
	default:
		break;
	}
	_sthChanged = true;
}

void Container::AutoAlignElementsVertically(Align align)
{
	switch (align)
	{
	case UIElement::Align::START:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto pos = e.second->getPosition();
			pos.y = 0.f;
			e.second->setPosition(pos);
		}
		break;
	case UIElement::Align::MIDDLE:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto& pos = e.second->getPosition();
			auto newPos = ViewHelper::GetScaled(sf::FloatRect(0.f, 0.5f, 1.f, 1.f), e.second->GetGlobalBounds(), GetGlobalBounds());

			e.second->setPosition(pos.x, newPos.top - getPosition().y);
		}
		break;
	case UIElement::Align::END:
		for (auto& e : _uiElements)
		{
			if (e.second == nullptr) continue;

			auto pos = e.second->getPosition();
			auto bounds = e.second->GetGlobalBounds();
			auto size = GetGlobalBounds();

			pos.y = size.height - bounds.height;
			e.second->setPosition(pos);
		}
		break;
	default:
		break;
	}

	_sthChanged = true;
}

sf::FloatRect Container::GetElementsGlobalBounds() const
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

UIElement* Container::clone()
{
	return new Container(*this);
}

void Container::Update(bool tick, float delta)
{
	for (auto& e : _uiElements)
		if (e.second != nullptr)
			e.second->Update(tick, delta);
}

void Container::ForceRedraw()
{
	for (auto& e : _uiElements)
		if (e.second != nullptr)
			e.second->ForceRedraw();
	Container::Redraw();
}

bool Container::Redraw()
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
		_render.display();

		_sthChanged = false;
		_redrawHappened = true;
		return true;
	}
	return false;
}

void Container::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	for (auto& e : _uiElements)
	{
		if (e.second == nullptr) continue;

		e.second->ProcessEvent(ev, getInverseTransform().transformPoint(mousePos));
	}
}

std::vector<sf::Vector2f> Container::GetAllBoundsPoints() const
{
	std::vector<sf::Vector2f> output = UIElement::GetAllBoundsPoints();
	for (auto& e : _uiElements)
	{
		auto vec = e.second->GetAllBoundsPoints();
		output.insert(output.end(), std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
	}
	for (size_t i = 4; i < output.size(); i++)
		output[i] = getTransform().transformPoint(output[i]);
	return output;
}

std::vector<sf::Vector2f> Container::GetDeepestInFocusBoundsPoints() const
{
	std::vector<sf::Vector2f> output;
	for (auto& e : _uiElements)
	{
		if (e.second->GetInFocus() == true)
		{
			output = e.second->GetDeepestInFocusBoundsPoints();
			break;
		}
	}
	if (output.size() == 0)
		output = UIElement::GetAllBoundsPoints();
	else
		for (auto& p : output)
			p = getTransform().transformPoint(p);
	return output;
}
