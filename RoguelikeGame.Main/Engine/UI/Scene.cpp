#include "Scene.h"

void Scene::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	auto size = target.getSize();
	sf::VertexArray _background(sf::PrimitiveType::Quads, 4);
	_background[0].position = sf::Vector2f(0.f, 0.f);
	_background[1].position = sf::Vector2f(float(size.x), 0.f);
	_background[2].position = sf::Vector2f(float(size.x), float(size.y));
	_background[3].position = sf::Vector2f(0.f, float(size.y));
	_background[0].color = _backgroundColor;
	_background[1].color = _backgroundColor;
	_background[2].color = _backgroundColor;
	_background[3].color = _backgroundColor;
	target.draw(_background);

	sf::RenderStates rs;
	for (auto& element : _uiElements)
	{
		if (element.second->GetVisibility() == false) continue;
		auto texture = element.second->GetTexture();
		size = texture->getSize();
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
	ClearFocus();
	_backgroundColor = sf::Color(0, 0, 0, 0);
}

Scene::Scene(Scene& other)
{
	_backgroundColor = other._backgroundColor;
	for (auto& e : other._uiElements)
		_uiElements[e.first] = e.second->clone();
	auto found = _uiElements.find(std::get<0>(other._inFocus));
	if (found != _uiElements.end())
		_inFocus = std::tuple<std::string, UIElement*>(found->first, found->second);
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

	for (auto &pair : _uiElements)
	{
		if (pair.second != nullptr)
			if (pair.second->GetMouseInput() == true || pair.second->GetKeyboardInput() == true)
				if (pair.second->GetFocusOnHover() == true || clicked == true)
					if (pair.second->GetGlobalBounds().contains(mousePos))
					{
						if (std::get<1>(_inFocus) != nullptr)
							std::get<1>(_inFocus)->SetInFocus(false);
						_inFocus = pair;
						std::get<1>(_inFocus)->SetInFocus(true);
						return;
					}
				
	}
	if (std::get<1>(_inFocus) != nullptr)
		if (std::get<1>(_inFocus)->GetFocusOnHover() == true || clicked == true)
			ClearFocus();
}

void Scene::UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	for (auto& e : _uiElements)
		e.second->ProcessEvent(ev, mousePos);
}

void Scene::Update(bool tick, float delta)
{
	for (auto& p : _uiElements)
		p.second->Update(tick, delta);
}

void Scene::RefreshElements()
{
	for (auto& e : _uiElements)
		e.second->ForceRedraw();
}

void Scene::AddElement(const std::string& name, UIElement* element)
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
	for(auto& p : _uiElements)
		if (p.second == element)
		{
			if (p.second != nullptr)
				delete p.second;
			_uiElements.erase(p.first);
		}
}

void Scene::RemoveElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
	{
		if (found->second != nullptr)
			delete found->second;
		_uiElements.erase(found);
	}
}

UIElement* Scene::GetElement(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second;
	else
		return nullptr;
}

const std::map<std::string, UIElement*>& Scene::GetElements() const
{
	return _uiElements;
}

const std::tuple<std::string, UIElement*>& Scene::GetFocused() const
{
	return _inFocus;
}

size_t Scene::GetNoOfElements() const
{
	return _uiElements.size();
}

sf::FloatRect Scene::GetElementsGlobalBounds() const
{
	if (_uiElements.size() == 0)
		return { 0.f,0.f,0.f,0.f };

	sf::Vector2f min(float(INT_MAX), float(INT_MAX)), max(float(INT_MIN), float(INT_MIN));
	for (auto& e : _uiElements)
		if (e.second != nullptr)
		{
			auto bounds = CollisionHelper::GetRectPoints(e.second->GetGlobalBounds());
			if (min.x > bounds[0].x) min.x = bounds[0].x;
			if (min.y > bounds[0].y) min.y = bounds[0].y;
			if (max.x < bounds[2].x) max.x = bounds[2].x;
			if (max.y < bounds[2].y) max.y = bounds[2].y;
		}
	return sf::FloatRect(min, max - min);
}

void Scene::SetBackgroundColor(const sf::Color& color)
{
	_backgroundColor = color;
}

void Scene::SetElementVisibility(const std::string& name, bool visible)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		found->second->SetVisibility(visible);
}

void Scene::SetFocus(const std::string& name)
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		_inFocus = std::tuple<std::string, UIElement*>(found->first, found->second);
}

void Scene::ClearFocus()
{
	if (std::get<1>(_inFocus) != nullptr)
	{
		std::get<1>(_inFocus)->SetInFocus(false);
		auto ev = new sf::Event;
		ev->type = sf::Event::MouseMoved;
		std::get<1>(_inFocus)->ProcessEvent(ev, sf::Vector2f(-1.f, -1.f));
		delete ev;
	}
	_inFocus = std::tuple<std::string, UIElement*>("", nullptr);
}

const sf::Color& Scene::GetBackgroundColor() const
{
	return _backgroundColor;
}

bool Scene::GetElementVisibility(const std::string& name) const
{
	auto found = _uiElements.find(name);
	if (found != _uiElements.end())
		return found->second->GetVisibility();
	return false;
}
