#include "Settings.h"

#define IF_EXIST_ASSIGN(doc, search, value) {auto found = doc.find(search); if(found != doc.end()) found->get_to(value);} 
Settings* Settings::_settings = nullptr;

Settings::Settings()
{
	DEBUG = false;
	SCALE_RATIO = 1.f;

	WINDOW_STYLE = 5;
	WINDOW_SIZE = sf::Vector2u(1024, 576);
	ANTIALIASING_LEVEL = 0;

	VSYNC_ENABLED = true;
	FRAMERATE_LIMIT = 0;

	MUSIC_VOLUME = 100.f;
	SOUNDS_VOLUME = 100.f;

	MOVE_UP = sf::Keyboard::Up;
	MOVE_DOWN = sf::Keyboard::Down;
	MOVE_LEFT = sf::Keyboard::Left;
	MOVE_RIGHT = sf::Keyboard::Right;
}

Settings* Settings::GetInstance()
{
	if (_settings == nullptr)
		_settings = new Settings();
	return _settings;
}

Settings::~Settings()
{
	;
}

bool Settings::LoadSettings(const std::string& path)
{
	std::ifstream stream;
	stream.open(path, std::ios::in);
	if (stream.is_open() == false || stream.good() == false)
		return false;

	nlohmann::json doc;
	stream >> doc;

	stream.close();

	IF_EXIST_ASSIGN(doc, "WINDOW_STYLE", WINDOW_STYLE.data);
	IF_EXIST_ASSIGN(doc, "WINDOW_SIZE_X", WINDOW_SIZE.data.x);
	IF_EXIST_ASSIGN(doc, "WINDOW_SIZE_Y", WINDOW_SIZE.data.y);
	IF_EXIST_ASSIGN(doc, "ANTIALIASING_LEVEL", ANTIALIASING_LEVEL.data);

	IF_EXIST_ASSIGN(doc, "VSYNC_ENABLED", VSYNC_ENABLED.data);
	IF_EXIST_ASSIGN(doc, "FRAMERATE_LIMIT", FRAMERATE_LIMIT.data);
				   
	IF_EXIST_ASSIGN(doc, "MUSIC_VOLUME", MUSIC_VOLUME.data);
	IF_EXIST_ASSIGN(doc, "SOUNDS_VOLUME", SOUNDS_VOLUME.data);
				   
	IF_EXIST_ASSIGN(doc, "MOVE_UP", MOVE_UP.data);
	IF_EXIST_ASSIGN(doc, "MOVE_DOWN", MOVE_DOWN.data);
	IF_EXIST_ASSIGN(doc, "MOVE_LEFT", MOVE_LEFT.data);
	IF_EXIST_ASSIGN(doc, "MOVE_RIGHT", MOVE_RIGHT.data);

	SCALE_RATIO = float(WINDOW_SIZE.data.x) / 1024.f;

	return true;
}

bool Settings::SaveSettings(const std::string& path)
{
	nlohmann::json doc
	{
		{"WINDOW_STYLE", WINDOW_STYLE.data},
		{"WINDOW_SIZE_X", WINDOW_SIZE.data.x},
		{"WINDOW_SIZE_Y", WINDOW_SIZE.data.y},
		{"ANTIALIASING_LEVEL", ANTIALIASING_LEVEL.data},
		{"VSYNC_ENABLED", VSYNC_ENABLED.data},
		{"FRAMERATE_LIMIT", FRAMERATE_LIMIT.data},
		{"MUSIC_VOLUME", MUSIC_VOLUME.data},
		{"SOUNDS_VOLUME", SOUNDS_VOLUME.data},
		{"MOVE_UP", MOVE_UP.data},
		{"MOVE_DOWN", MOVE_DOWN.data},
		{"MOVE_LEFT", MOVE_LEFT.data},
		{"MOVE_RIGHT", MOVE_RIGHT.data}
	};

	std::ofstream stream;
	stream.open(path, std::ios::trunc | std::ios::out);
	if (stream.is_open() == false || stream.good() == false)
		return false;

	stream << doc.dump(0);
	stream.close();

	return true;
}
