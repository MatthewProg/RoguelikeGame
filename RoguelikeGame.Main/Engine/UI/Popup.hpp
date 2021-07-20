#pragma once

#include "SFML/Graphics/Transformable.hpp"

#include "../Handlers/ResultHandler.hpp"
#include "../UI/Scene.h"

template<class T>
class Popup : public Scene, public sf::Transformable
{
private:
	T _popupResult;
	ResultHandler<T>* _resultHandler;

	bool _canClose;

	void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	Popup();
	Popup(Popup<T>& other);
	~Popup() override;

	void UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	void Update(bool tick, float delta) override;

	void SetResultHandler(ResultHandler<T>* handler);
	void SetCanClose(bool close);

	const T* GetResult() const;
	bool HasResult() const;
	bool CanClose() const;

	sf::FloatRect GetElementsGlobalBounds() const override;
};

template<class T>
inline void Popup<T>::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	auto size = target.getSize();
	auto& backgroundColor = Scene::GetBackgroundColor();
	sf::VertexArray _background(sf::PrimitiveType::Quads, 4);
	_background[0].position = sf::Vector2f(0.f, 0.f);
	_background[1].position = sf::Vector2f(float(size.x), 0.f);
	_background[2].position = sf::Vector2f(float(size.x), float(size.y));
	_background[3].position = sf::Vector2f(0.f, float(size.y));
	_background[0].color = backgroundColor;
	_background[1].color = backgroundColor;
	_background[2].color = backgroundColor;
	_background[3].color = backgroundColor;
	target.draw(_background);

	sf::RenderStates rs;
	auto& elemments = Scene::GetElements();
	for (auto& element : elemments)
	{
		if (element.second->GetVisibility() == false) continue;
		auto texture = element.second->GetTexture();
		size = texture->getSize();
		rs.texture = &texture->getTexture();
		rs.transform = getTransform();
		rs.transform.combine(element.second->getTransform());
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

template<class T>
inline Popup<T>::Popup() : Scene()
{
	_resultHandler = nullptr;
	_popupResult = T();
	_canClose = false;
}

template<class T>
inline Popup<T>::Popup(Popup<T>& other) : Scene::Scene(other), Transformable(other)
{
	_popupResult = other._popupResult;
	_resultHandler = other._resultHandler->clone();
	_canClose = other._canClose;
}

template<class T>
inline Popup<T>::~Popup()
{
	if (_resultHandler != nullptr)
		delete _resultHandler;
}

template<class T>
inline void Popup<T>::UpdateEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	Scene::UpdateEvent(ev, mousePos);

	if (_resultHandler != nullptr)
	{
		_resultHandler->UpdateResult(&_popupResult);
		if (ev->type == sf::Event::KeyReleased && ev->key.code == sf::Keyboard::Escape)
			_canClose = true;
	}
}

template<class T>
inline void Popup<T>::Update(bool tick, float delta)
{
	Scene::Update(tick, delta);

	if (_resultHandler != nullptr)
		_resultHandler->UpdateResult(&_popupResult);
}

template<class T>
inline void Popup<T>::SetResultHandler(ResultHandler<T>* handler)
{
	if (_resultHandler != nullptr)
		delete _resultHandler;
	_resultHandler = handler;
}

template<class T>
inline void Popup<T>::SetCanClose(bool close)
{
	_canClose = close;
}

template<class T>
inline const T* Popup<T>::GetResult() const
{
	return &_popupResult;
}

template<class T>
inline bool Popup<T>::HasResult() const
{
	if (_resultHandler != nullptr)
		return _resultHandler->GetGotResult();
	return false;
}

template<class T>
inline bool Popup<T>::CanClose() const
{
	return _canClose;
}

template<class T>
inline sf::FloatRect Popup<T>::GetElementsGlobalBounds() const
{
	return getTransform().transformRect(Scene::GetElementsGlobalBounds());
}