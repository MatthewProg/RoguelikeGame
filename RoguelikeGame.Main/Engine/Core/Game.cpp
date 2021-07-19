#include "Game.h"

Game::Game(LogOptions options) : _logger(Logger::GetInstance(options)), _keyboardHandler(this)
{
	_settings = Settings::GetInstance();

	_camera.setCenter(128, 64);
	_camera.setSize(256.f, 144.f);
	_gui.setCenter(512.f, 288.f);
	_gui.setSize(1024.f, 576.f);
	_gui.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	_delta = 1.0000000;
	_tickCounter = 0.0;
	_gameSpeed = 60;
	_inGame = false;
	_lastFrameTime = std::chrono::steady_clock::now();
	_event = sf::Event();
	_player = nullptr;

	srand((uint32_t)time(NULL));
}

void Game::SetDeltaAndTick()
{
	if (_tickCounter >= 1)
		_tickCounter -= 1;

	std::chrono::steady_clock::time_point newFrame = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(newFrame - _lastFrameTime).count();
	_lastFrameTime = newFrame;

	_delta = (dur / 1000000000.000) * _gameSpeed;
	_tickCounter += _delta;
}

void Game::RecalcPlayerRays()
{
	auto coord = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
	auto angle = MathHelper::GetAngleBetweenPoints(ViewHelper::GetRectCenter(_player->GetCollisionBox()), sf::Vector2f(coord.x, coord.y));
	_player->GetWeapon()->SetCurrentAngle(angle);
	_player->GetWeapon()->SetRaycastHitpoint(_collisionsManager.GetRayHitpoint(ViewHelper::GetRectCenter(_player->GetCollisionBox()), angle, 500));
}

void Game::UpdateUI()
{
	if (_inGame)
	{
		auto sc = _sceneManager.GetScene("game");
		ProgressBar* hb = (ProgressBar*)sc->GetElement("healthBar");
		hb->SetCurrentValue(_player->GetHealth());
	}
	auto ls = _sceneManager.GetLoadedScene();
	if (ls != nullptr && _sceneManager.GetLoadedSceneName() == "options")
	{
		if (UIHelper::ExtractCheckBox(ls, "view", "vsync", "checkbox")->IsChecked() == true)
			(((ScrollView*)ls->GetElement("view"))->GetElement("fps"))->SetEnabled(false);
		else
			(((ScrollView*)ls->GetElement("view"))->GetElement("fps"))->SetEnabled(true);
	}
	CheckButtons();
}

void Game::UpdateGame()
{
	_playerMovement.Update((float)_delta);
	_player->Update(Game::Tick(), (float)_delta);
	if (_playerMovement.IsKeyPressed()) RecalcPlayerRays();

	_enemies.Update(Game::Tick(), (float)_delta);
	_enemies.CheckAttacks();
	_enemiesAI.Update((float)_delta);

	_camera.setCenter(ViewHelper::GetRectCenter(_player->GetCollisionBox()));
}

void Game::CheckButtons()
{
	auto loaded = _sceneManager.GetLoadedScene();
	if (loaded != nullptr)
	{
		if (_sceneManager.GetLoadedSceneName() == "main_menu")
		{
			if (((Button*)loaded->GetElement("exit"))->Clicked()) Close();
			else if (((Button*)loaded->GetElement("options"))->Clicked()) { _sceneManager.LoadScene("options"); }
			else if (((Button*)loaded->GetElement("play"))->Clicked()) { LoadLevel("./res/maps/map1.json", "male_elf"); }
		}
		else if (_sceneManager.GetLoadedSceneName() == "options")
		{
			/*auto ml = UIHelper::ExtractButton(loaded, "view", "move_left", "text"); //USE WHEN CHANGING KEYS
			auto mr = UIHelper::ExtractButton(loaded, "view", "move_right", "text");
			auto mu = UIHelper::ExtractButton(loaded, "view", "move_up", "text");
			auto md = UIHelper::ExtractButton(loaded, "view", "move_down", "text");*/

			if (((Button*)loaded->GetElement("save_button"))->Clicked()) { SaveSettings(); ApplySettings(); _sceneManager.LoadScene("main_menu"); }
		}
	}
}

void Game::SaveSettings()
{
	auto sc = _sceneManager.GetScene("options");
	if (sc == nullptr) return;

	auto displayMode = UIHelper::ExtractListSelect(sc, "view", "display_mode", "listselect");
	auto resolution = UIHelper::ExtractListSelect(sc, "view", "resolution", "listselect");
	auto vsync = UIHelper::ExtractCheckBox(sc, "view", "vsync", "checkbox");
	auto fps = UIHelper::ExtractListSelect(sc, "view", "fps", "listselect");
	auto antialiasing = UIHelper::ExtractListSelect(sc, "view", "antialiasing", "listselect");
	auto sound_volume = UIHelper::ExtractProgressBar(sc, "view", "sound_volume", "bar");
	auto music_volume = UIHelper::ExtractProgressBar(sc, "view", "music_volume", "bar");
	auto mLeft = UIHelper::ExtractButton(sc, "view", "move_left", "text");
	auto mRight = UIHelper::ExtractButton(sc, "view", "move_right", "text");
	auto mUp = UIHelper::ExtractButton(sc, "view", "move_up", "text");
	auto mDown = UIHelper::ExtractButton(sc, "view", "move_down", "text");

	if (displayMode != nullptr)
	{
		if(displayMode->GetCurrentValue() == "Windowed") _settings->WINDOW_STYLE.NewValue(5);
		else if(displayMode->GetCurrentValue() == "Borderless") _settings->WINDOW_STYLE.NewValue(0);
		else if(displayMode->GetCurrentValue() == "Fullscreen") _settings->WINDOW_STYLE.NewValue(8);
	}
	if (resolution != nullptr)
	{
		auto s = resolution->GetCurrentValue();
		auto index = s.find('x');
		auto width = atoi(s.substr(0, index).c_str());
		auto height = atoi(s.substr(index + 1).c_str());
		_settings->WINDOW_SIZE.NewValue(sf::Vector2u(uint32_t(width), uint32_t(height)));
	}
	if (vsync != nullptr) _settings->VSYNC_ENABLED.NewValue(vsync->IsChecked());
	if (fps != nullptr) _settings->FRAMERATE_LIMIT.NewValue(uint32_t(atoi(fps->GetCurrentValue().c_str())));
	if (antialiasing != nullptr) _settings->ANTIALIASING_LEVEL.NewValue(uint32_t(atoi(antialiasing->GetCurrentValue().c_str())));
	if (sound_volume != nullptr) _settings->SOUNDS_VOLUME.NewValue(sound_volume->GetCurrentValue());
	if (music_volume != nullptr)_settings->MUSIC_VOLUME.NewValue(music_volume->GetCurrentValue());
	if (mLeft != nullptr) _settings->MOVE_LEFT.NewValue(InputHelper::EncodeKey(mLeft->EditTextState("none")->getString()));
	if (mRight != nullptr) _settings->MOVE_RIGHT.NewValue(InputHelper::EncodeKey(mRight->EditTextState("none")->getString()));
	if (mUp != nullptr) _settings->MOVE_UP.NewValue(InputHelper::EncodeKey(mUp->EditTextState("none")->getString()));
	if (mDown != nullptr) _settings->MOVE_DOWN.NewValue(InputHelper::EncodeKey(mDown->EditTextState("none")->getString()));

	if (_settings->SaveSettings("./settings.json") == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to save settings");
	else
		_logger->Log(Logger::LogType::DEBUG, "Settings saved!");
}

void Game::ApplySettings()
{
	_sounds.ApplyVolume(_settings->SOUNDS_VOLUME);
}

void Game::LoadLevel(std::string path, std::string playerTemplate)
{
	_logger->Log(Logger::LogType::INFO, "Loading level: " + path);

	//Game map
	_logger->Log(Logger::LogType::INFO, "Loading map components");
	if (_gameMap.LoadFromFile(path) == false)
		_logger->Log(Logger::LogType::ERROR, "Unable to load map data!");
	else
		_logger->Log(Logger::LogType::INFO, "Loaded map data");

	_gameMap.AutoSetTilesTextures(&_textures);
	_gameMap.PrepareFrame();

	//Collisions
	_collisionsManager.AddMap(*_gameMap.GetActionMap(), (unsigned char)1);
	_collisionsManager.GenerateCommonMap();
	_collisionsManager.CovertTilesIntoEdges();

	//Player
	_logger->Log(Logger::LogType::INFO, "Loading player components");
	_player = _objTemplates.GetPlayer(playerTemplate);
	auto camSize = _camera.getSize() + sf::Vector2f(44, 44);
	_player->SetView(sf::FloatRect(0 - (camSize.x / 2), 0 - (camSize.y / 2), camSize.x, camSize.y));

	//Player movement
	_playerMovement.SetIdleStateName("idle");
	_playerMovement.SetMoveStateName("move");
	_playerMovement.SetEntity(_player);
	_playerMovement.SetCollisionsManager(&_collisionsManager);

	//Enemies AI
	_enemiesAI.SetTarget(_player);
	_enemiesAI.SetCollisionsManager(&_collisionsManager);
	_enemiesAI.SetEnemiesManager(&_enemies);
	_enemiesAI.SetPathfindPoints(_gameMap.GetPathfindingPoints());

	//Enemies
	_enemies.SetPlayer(_player);
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	/*_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));
	_enemies.Add(_objTemplates.GetEnemy("devil"));*/
	_enemies.GetEnemies()->at(0)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	/*_enemies.GetEnemies()->at(1)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(2)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));
	_enemies.GetEnemies()->at(3)->SetWeapon(_objTemplates.GetHitboxWeapon("bite"));*/
	_enemies.GetEnemies()->at(0)->SetPosition(500, 290);
	/*_enemies.GetEnemies()->at(1)->SetPosition(580, 300);
	_enemies.GetEnemies()->at(2)->SetPosition(590, 310);
	_enemies.GetEnemies()->at(3)->SetPosition(610, 300);*/

	//Sounds
	_sounds.LoadFromFile("entities_dmg4", "./res/sounds/entities/dmg4.wav");
	_sounds.LoadFromFile("entities_dmg5", "./res/sounds/entities/dmg5.wav");
	_sounds.LoadFromFile("entities_dmg6", "./res/sounds/entities/dmg6.wav");
	_sounds.LoadFromFile("weapons_swing1", "./res/sounds/weapons/swing1.wav");
	_sounds.LoadFromFile("weapons_swing2", "./res/sounds/weapons/swing2.wav");
	_sounds.LoadFromFile("weapons_swing3", "./res/sounds/weapons/swing3.wav");
	_sounds.LoadFromFile("weapons_bite1", "./res/sounds/weapons/bite1.wav");

	//Scene
	_sceneManager.LoadScene("game");

	_inGame = true;
	_logger->Log(Logger::LogType::INFO, "Done");
}

bool Game::Tick()
{
	if (_tickCounter >= 1)
		return true;
	return false;
}

void Game::Close()
{
	_logger->Log(Logger::LogType::INFO, "Closing...");
	_window.close();
}

Game::~Game()
{
	delete _player;
}

void Game::Start()
{
	_logger->Log(Logger::LogType::INFO, "Starting...");

	//Settings
	if (_settings->LoadSettings("./settings.json"))
		_logger->Log(Logger::LogType::INFO, "Loaded settings");
	else
	{
		_logger->Log(Logger::LogType::ERROR, "Unable to load settings");
		_logger->Log(Logger::LogType::WARNING, "Starting with default values");
		if (_settings->SaveSettings("./settings.json"))
			_logger->Log(Logger::LogType::INFO, "Created new settings file");
		else
			_logger->Log(Logger::LogType::ERROR, "Unable to create settings file");
	}

	//Window
	sf::Vector2u winSize = _settings->WINDOW_SIZE;
	sf::ContextSettings cs;
	cs.antialiasingLevel = _settings->ANTIALIASING_LEVEL;
	cs.attributeFlags = ((_settings->DEBUG) ? sf::ContextSettings::Attribute::Debug : sf::ContextSettings::Attribute::Default);
	_window.create(sf::VideoMode(winSize.x, winSize.y), "It's a game", _settings->WINDOW_STYLE, cs);
	if (_settings->VSYNC_ENABLED)
	{
		_window.setVerticalSyncEnabled(true);
		_window.setFramerateLimit(0U);
	}
	else
	{
		_window.setVerticalSyncEnabled(false);
		_window.setFramerateLimit(_settings->FRAMERATE_LIMIT);
	}

	//Cameras
	_camera.setSize(256.f, 144.f);
	_gui.setCenter(((float)winSize.x / 2.f), ((float)winSize.y / 2.f));
	_gui.setSize((float)winSize.x, (float)winSize.y);
	_window.setView(_camera);

	//Textures
	_textures.LoadFromFile("tiles1", "./res/img/tiles.png");
	_textures.LoadFromFile("tiles2", "./res/img/tiles2.png");
	_textures.LoadFromFile("players", "./res/img/players.png");
	_textures.LoadFromFile("special", "./res/img/special.png");
	_textures.LoadFromFile("ui", "./res/img/ui.png");
	_textures.ApplySmooth(false);
	_textures.ApplyRepeat(false);

	//Fonts
	_fonts.LoadFromFile("menu", "./res/fonts/menu.ttf");

	//Sounds
	_sounds.LoadFromFile("ui_click", "./res/sounds/ui/click.wav");
	_sounds.LoadFromFile("ui_coin", "./res/sounds/ui/coin.wav");
	_sounds.LoadFromFile("ui_hit", "./res/sounds/ui/hit.wav");
	_sounds.LoadFromFile("ui_release", "./res/sounds/ui/release.wav");
	_sounds.LoadFromFile("ui_select", "./res/sounds/ui/select.wav");

	//Object manager
	_objTemplates.SetTexturesManager(&_textures);
	_objTemplates.SetFontsManager(&_fonts);
	_objTemplates.SetSoundsManager(&_sounds);
	_objTemplates.SetWindowSize(_window.getSize());

	//UI elements
	_sceneManager.AddScene("game", _objTemplates.GetScene("game"));
	_sceneManager.AddScene("main_menu", _objTemplates.GetScene("main_menu"));
	_sceneManager.AddScene("options", _objTemplates.GetScene("options"));
	_sceneManager.LoadScene("main_menu");
	_sceneManager.SetShowFocused(false);

	//Update options scene
	auto opt = _sceneManager.GetScene("options");
	auto displayMode = UIHelper::ExtractListSelect(opt, "view", "display_mode", "listselect");
	auto resolution = UIHelper::ExtractListSelect(opt, "view", "resolution", "listselect");
	auto vsync = UIHelper::ExtractCheckBox(opt, "view", "vsync", "checkbox");
	auto fps = UIHelper::ExtractListSelect(opt, "view", "fps", "listselect");
	auto antialiasing = UIHelper::ExtractListSelect(opt, "view", "antialiasing", "listselect");
	auto sound_volume = UIHelper::ExtractProgressBar(opt, "view", "sound_volume", "bar");
	auto music_volume = UIHelper::ExtractProgressBar(opt, "view", "music_volume", "bar");
	auto mLeft = UIHelper::ExtractButton(opt, "view", "move_left", "text");
	auto mRight = UIHelper::ExtractButton(opt, "view", "move_right", "text");
	auto mUp = UIHelper::ExtractButton(opt, "view", "move_up", "text");
	auto mDown = UIHelper::ExtractButton(opt, "view", "move_down", "text");
	if (displayMode != nullptr)
	{
		std::string toSet = "Windowed";
		if (_settings->WINDOW_STYLE == 5U) toSet = "Windowed";
		else if (_settings->WINDOW_STYLE == 0U) toSet = "Borderless";
		else if (_settings->WINDOW_STYLE == 8U) toSet = "Fullscreen";
		auto& values = displayMode->GetValues();
		for (size_t i = 0; i < values.size(); i++)
			if (values[i] == toSet)
			{
				displayMode->SetCurrentIndex(i);
				break;
			}
	}
	if (resolution != nullptr)
	{
		auto res = std::to_string(winSize.x) + "x" + std::to_string(winSize.y);
		auto& values = resolution->GetValues();
		bool done = false;
		for (size_t i = 0; i < values.size(); i++)
			if (values[i] == res)
			{
				resolution->SetCurrentIndex(i);
				done = true;
				break;
			}
		if (done == false)
		{
			resolution->AddValue(res);
			resolution->SetCurrentIndex(resolution->GetValuesSize() - 1);
		}
	}
	if (vsync != nullptr)vsync->SetChecked(_settings->VSYNC_ENABLED);
	if (fps != nullptr)
	{
		auto limit = std::to_string(_settings->FRAMERATE_LIMIT);
		if (limit != "0")
		{
			auto& values = fps->GetValues();
			bool done = false;
			for (size_t i = 0; i < values.size(); i++)
				if (values[i] == limit)
				{
					fps->SetCurrentIndex(i);
					done = true;
					break;
				}
			if (done == false)
			{
				fps->AddValue(limit);
				fps->SetCurrentIndex(fps->GetValuesSize() - 1);
			}
		}
		else
			fps->SetCurrentIndex(0U);
	}
	if (antialiasing != nullptr)
	{
		auto level = std::to_string(_settings->ANTIALIASING_LEVEL);
		auto& values = antialiasing->GetValues();
		bool done = false;
		for (size_t i = 0; i < values.size(); i++)
			if (values[i] == level)
			{
				antialiasing->SetCurrentIndex(i);
				done = true;
				break;
			}
		if (done == false)
		{
			antialiasing->AddValue(level);
			antialiasing->SetCurrentIndex(antialiasing->GetValuesSize() - 1);
		}
	}
	if (sound_volume != nullptr) sound_volume->SetCurrentValue(_settings->SOUNDS_VOLUME);
	if (music_volume != nullptr) music_volume->SetCurrentValue(_settings->MUSIC_VOLUME);
	if (mLeft != nullptr)
	{
		mLeft->ApplyText(InputHelper::DecodeKey(_settings->MOVE_LEFT));
		auto btnBounds = mLeft->EditTextState("none")->getGlobalBounds();
		mLeft->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
		mLeft->setPosition(914.f - btnBounds.width - btnBounds.left - (10.f * _settings->SCALE_RATIO), 0.f);
		((ScrollView*)opt->GetElement("view"))->GetElement("move_left")->AutoAlignElementsVertically(UIElement::Align::MIDDLE);
	}
	if (mRight != nullptr) 
	{
		mRight->ApplyText(InputHelper::DecodeKey(_settings->MOVE_RIGHT));
		auto btnBounds = mRight->EditTextState("none")->getGlobalBounds();
		mRight->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
		mRight->setPosition(914.f - btnBounds.width - btnBounds.left - (10.f * _settings->SCALE_RATIO), 0.f);
		((ScrollView*)opt->GetElement("view"))->GetElement("move_right")->AutoAlignElementsVertically(UIElement::Align::MIDDLE);
	}
	if (mUp != nullptr)
	{
		mUp->ApplyText(InputHelper::DecodeKey(_settings->MOVE_UP));
		auto btnBounds = mUp->EditTextState("none")->getGlobalBounds();
		mUp->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
		mUp->setPosition(914.f - btnBounds.width - btnBounds.left - (10.f * _settings->SCALE_RATIO), 0.f);
		((ScrollView*)opt->GetElement("view"))->GetElement("move_up")->AutoAlignElementsVertically(UIElement::Align::MIDDLE);
	}
	if (mDown != nullptr)
	{
		mDown->ApplyText(InputHelper::DecodeKey(_settings->MOVE_DOWN));
		auto btnBounds = mDown->EditTextState("none")->getGlobalBounds();
		mDown->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
		mDown->setPosition(914.f - btnBounds.width - btnBounds.left - (10.f * _settings->SCALE_RATIO), 0.f);
		((ScrollView*)opt->GetElement("view"))->GetElement("move_down")->AutoAlignElementsVertically(UIElement::Align::MIDDLE);
	}

	//Debug
	_gameMap.SetActionMapOpacity(0.25);

	//Keybinds
	if (_settings->DEBUG)
	{
		sf::Event::KeyEvent ctrlAltG = { sf::Keyboard::G, true, true };
		sf::Event::KeyEvent ctrlAltA = { sf::Keyboard::A, true, true };
		sf::Event::KeyEvent ctrlAltH = { sf::Keyboard::H, true, true };
		sf::Event::KeyEvent ctrlAltD = { sf::Keyboard::D, true, true };
		sf::Event::KeyEvent ctrlAltR = { sf::Keyboard::R, true, true };
		sf::Event::KeyEvent ctrlAltP = { sf::Keyboard::P, true, true };
		sf::Event::KeyEvent ctrlAltN = { sf::Keyboard::N, true, true };
		sf::Event::KeyEvent ctrlAltU = { sf::Keyboard::U, true, true };
		_keyboardHandler.NewOn(ctrlAltG, &Game::ToggleGridVisibility);
		_keyboardHandler.NewOn(ctrlAltA, &Game::ToggleActionMapVisibility);
		_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleHitboxVisibility);
		_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleEnemiesHitboxVisibility);
		_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleMapCollisionLinesVisibility);
		_keyboardHandler.NewOn(ctrlAltH, &Game::ToggleWeaponHitboxVisibility);
		_keyboardHandler.NewOn(ctrlAltD, &Game::ToggleConsoleInfo);
		_keyboardHandler.NewOn(ctrlAltR, &Game::ToggleRaycastVisibility);
		_keyboardHandler.NewOn(ctrlAltP, &Game::TogglePathfindingVisibility);
		_keyboardHandler.NewOn(ctrlAltN, &Game::ToggleNoClip);
		_keyboardHandler.NewOn(ctrlAltU, &Game::ToggleUIFrames);
	}

	//Reset timings
	SetDeltaAndTick();
}

void Game::EventUpdate()
{
	bool LMB_Clicked = false;
	_window.setView(_gui);
	auto mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
	_window.setView(_camera);
	while (_window.pollEvent(_event))
	{
		if (_event.type == sf::Event::Closed)
			Close();	
		if (_event.type == sf::Event::KeyPressed)
			_keyboardHandler.Rise(_event.key);
		if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
		{
			if (_inGame)
			{
				if (_player->GetWeapon()->CanAttack())
				{
					_player->GetWeapon()->Attack();
					_enemies.CheckForHit();
				}
			}
			LMB_Clicked = true;
		}
		if (_event.type == sf::Event::MouseMoved)
		{
			if (_inGame)
				RecalcPlayerRays();
		}

		_sceneManager.UpdateEvent(&_event, mousePos);
	}
	_sceneManager.UpdateFocus(mousePos, LMB_Clicked);
}

void Game::Update()
{
	SetDeltaAndTick();
	_debug.Status(Game::Tick());

	if (_inGame)
		UpdateGame();

	UpdateUI();
	_sceneManager.Update(Game::Tick(), (float)_delta);
	_sounds.Update();
}

void Game::Clear()
{
	_window.clear(sf::Color::Transparent);
}

void Game::Draw()
{
	if (_inGame)
	{
		_window.setView(_camera);
		_window.draw(_gameMap);
		_window.draw(_collisionsManager);
		_window.draw(_enemiesAI);
		_window.draw(_enemies);
		_window.draw(*_player);
	}

	_window.setView(_gui);
	_window.draw(_sceneManager);

	_window.setView(_camera);
}

void Game::Display()
{
	_window.display();
}

bool Game::isRunning()
{
	return _window.isOpen();
}

#pragma region Events
void Game::ToggleGridVisibility()
{
	_gameMap.ToggleGridVisibility();
}

void Game::ToggleActionMapVisibility()
{
	_gameMap.ToggleActionMapVisibility();
}

void Game::ToggleHitboxVisibility()
{
	if(_player != nullptr)
		_player->ToggleHitboxVisibility();
}

void Game::ToggleConsoleInfo()
{
	_debug.ToggleFPSInfo();
}

void Game::ToggleWeaponHitboxVisibility()
{
	if(_player != nullptr)
		_player->ToggleWeaponHitboxVisibility();
}

void Game::ToggleEnemiesHitboxVisibility()
{
	if (_player != nullptr)
		_enemies.ToggleEnemiesHitboxVisibility();
}

void Game::ToggleMapCollisionLinesVisibility()
{
	if (_player != nullptr)
		_collisionsManager.ToggleCollisionLinesVisibility();
}

void Game::ToggleRaycastVisibility()
{
	if (_player != nullptr)
	{
		_player->ToggleRaycastVisibility();
		_enemies.ToggleEnemiesRaycastVisibility();
	}
}

void Game::TogglePathfindingVisibility()
{
	_enemiesAI.TogglePathfindingVisibility();
}

void Game::ToggleNoClip()
{
	_playerMovement.ToggleNoClip();
}

void Game::ToggleUIFrames()
{
	_sceneManager.ToggleShowFocused();
}

#pragma endregion

