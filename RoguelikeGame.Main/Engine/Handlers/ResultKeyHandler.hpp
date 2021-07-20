#pragma once

#include "../Handlers/ResultHandler.hpp"

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Event.hpp"

template<class T>
class ResultKeyHandler : public ResultHandler<T>
{
private:
	const sf::Event* _event;
public:
	ResultKeyHandler(sf::Event* ev) : _event(ev) { ; }
	ResultKeyHandler(ResultKeyHandler<T>& other) : ResultHandler<T>(other) { _event = other._event; }
	~ResultKeyHandler() override = default;

	ResultHandler<T>* clone() override
	{
		return new ResultKeyHandler<T>(*this);
	}

	// Inherited via ResultHandler
	bool UpdateResult(T* result) override
	{
		if (_event->type == sf::Event::KeyReleased)
		{
			if (_event->key.code == sf::Keyboard::Escape)
			{
				ResultHandler<T>::SetGotResult(false);
				return false;
			}
			else
			{
				*result = _event->key.code;
				ResultHandler<T>::SetGotResult(true);
				return true;
			}
		}
		return false;
	}
};

