#pragma once

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Graphics.hpp"

class Settings
{
protected:
	Settings();
	static Settings* _settings;
public:
	template <class T>
	class Option { //Read-only from outside
		friend class Settings;
	private:
		T data;
		T operator=(const T& arg) { data = arg; return data; }
	public:
		operator const T& () const { return data; }
		void NewValue(T value) { data = value; }
	};

	Settings(Settings& other) = delete;
	void operator=(const Settings&) = delete;

	static Settings* GetInstance();
	~Settings();

	bool LoadSettings(std::string path);
	bool SaveSettings(std::string path);

	//Only runtime, it's not stored in file
	Option<bool> DEBUG;

	Option<uint32_t> WINDOW_STYLE;
	Option<sf::Vector2u> WINDOW_SIZE;
	Option<uint32_t> ANTIALIASING_LEVEL;

	Option<bool> VSYNC_ENABLED;
	Option<uint32_t> FRAMERATE_LIMIT;

	Option<float> MUSIC_VOLUME;
	Option<float> SOUNDS_VOLUME;

	Option<sf::Keyboard::Key> MOVE_UP;
	Option<sf::Keyboard::Key> MOVE_DOWN;
	Option<sf::Keyboard::Key> MOVE_LEFT;
	Option<sf::Keyboard::Key> MOVE_RIGHT;
};

