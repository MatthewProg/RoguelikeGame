#include "Scene.h"

void Scene::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	sf::RenderStates rs;
	for (auto& element : _uiElements)
	{
		if (element.second->GetVisibility() == false) continue;
		auto texture = element.second->GetTexture();
		auto size = texture->getSize();
		rs.texture = &texture->getTexture();
		rs.transform = element.second->getTransform();
		sf::VertexArray draw(sf::Quads, 4);
		draw[0].position = sf::Vector2f(0, 0);
		draw[1].position = sf::Vector2f((float)size.x, 0);
		draw[2].position = sf::Vector2f((float)size.x, (float)size.y);
		draw[3].position = sf::Vector2f(0, (float)size.y);
		draw[0].texCoords = sf::Vector2f(0, 0);
		draw[1].texCoords = sf::Vector2f((float)size.x, 0);
		draw[2].texCoords = sf::Vector2f((float)size.x, (float)size.y);
		draw[3].texCoords = sf::Vector2f(0, (float)size.y);
		target.draw(draw, rs);
	}
}

Scene::Scene()
{
	_uiElements.clear();
}

Scene::~Scene()
{
	for (auto &p : _uiElements)
		if(p.second != nullptr)
			delete p.second;
}

void Scene::UpdateFocus(const sf::Vector2f& mousePos, bool clicked)
{
	auto focused = std::get<1>(_inFocus);
	if (focused != nullptr)
		if (focused->GetGlobalBounds().contains(mousePos))
			return;

	for (auto pair : _uiElements)
	{
		if (pair.second != nullptr)
			if (pair.second->GetMouseInput() == true || pair.second->GetKeyboardInput() == true)
				if (pair.second->GetFocusOnHover() == true || clicked == true)
					if (pair.second->GetGlobalBounds().contains(mousePos))
					{
						_inFocus = pair;
						return;
					}
				
	}
	if (std::get<1>(_inFocus) != nullptr)
		if (std::get<1>(_inFocus)->GetFocusOnHover() == true || clicked == true)
			ClearFocus();
}

void Scene::UpdateEvent(sf::Event* ev, sf::Vector2f mousePos)
{
	auto focused = std::get<1>(_inFocus);
	if (focused != nullptr)
		focused->ProcessEvent(ev, mousePos);
}

void Scene::Update(bool tick, float delta)
{
	for (auto& p : _uiElements)
		p.second->Update(tick, delta);
}

void Scene::AddElement(std::string name, UIElement* element)
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

void Scene::RemoveElement(UIElement* element)
{
	for(auto p : _uiElements)
		if (p.second == element)
		{
			if (p.second != nullptr)
				delete p.second;
			_uiElements.erase(p.first);
		}
}

void Scene::RemoveElement(std::string name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
	}
}

UIElement* Scene::GetElement(std::string name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second;
	else
		return nullptr;
}

std::tuple<std::string, UIElement*> Scene::GetFocused()
{
	return _inFocus;
}

size_t Scene::GetNoOfElements()
{
	return _uiElements.size();
}

void Scene::SetElementVisibility(std::string name, bool visible)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		found->second->SetVisibility(visible);
}

void Scene::SetFocus(std::string name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		_inFocus = std::tuple<std::string, UIElement*>(found->first, found->second);
}

void Scene::ClearFocus()
{
	_inFocus = std::tuple<std::string, UIElement*>("", nullptr);
}

bool Scene::GetElementVisibility(std::string name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second->GetVisibility();
	return false;
}
