#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Utilities.h"

#include "SFML/Window/Event.hpp"

template<class T>
class KeyboardEventHandler
{
private:
	std::unordered_map<EventKeyMapKey, std::vector<void(T::*)()>, EventKeyMapKeyHasher> _events;
	T* _ptr;
public:
	KeyboardEventHandler(T* ptr)
	{
		_ptr = ptr;
	}

	void Rise(sf::Event::KeyEvent ev)
	{
		EventKeyMapKey key;
		key.alt = ev.alt;
		key.code = ev.code;
		key.control = ev.control;
		key.shift = ev.shift;
		key.system = ev.system;

		auto found = _events.find(key);
		if (found != _events.end())
		{
			for (size_t it = 0; it < found->second.size(); it++)
			{
				(*_ptr.*(found->second[it]))();
			}
		}
	}

	void NewOn(sf::Event::KeyEvent ev, void (T::* rise)())
	{
		EventKeyMapKey key;
		key.alt = ev.alt;
		key.code = ev.code;
		key.control = ev.control;
		key.shift = ev.shift;
		key.system = ev.system;
		_events[key].push_back(rise);
	}

	void DeleteOn(sf::Event::KeyEvent ev, void (T::* del)())
	{
		EventKeyMapKey key;
		key.alt = ev.alt;
		key.code = ev.code;
		key.control = ev.control;
		key.shift = ev.shift;
		key.system = ev.system;

		auto found = _events.find(key);
		if (found != _events.end())
		{
			auto it = std::find(found->second.begin(), found->second.end(), del);
			if (it != found->second.end())
				found->second.erase(it);
		}
	}

	void ClearOn(sf::Event::KeyEvent ev)
	{
		EventKeyMapKey key;
		key.alt = ev.alt;
		key.code = ev.code;
		key.control = ev.control;
		key.shift = ev.shift;
		key.system = ev.system;

		auto found = _events.find(key);
		if (found != _events.end())
			found->second.clear();
	}

	void Clear()
	{
		_events.clear();
	}
};