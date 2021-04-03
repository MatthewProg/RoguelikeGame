#include "ObjectsManager.h"

void ObjectsManager::SetTexturesManager(TexturesManager* textures)
{
	_textures = textures;
}

void ObjectsManager::SetFontsManager(FontsManager* fonts)
{
	_fonts = fonts;
}

void ObjectsManager::SetSoundsManager(SoundsManager* sounds)
{
	_sounds = sounds;
}

void ObjectsManager::SetWindowSize(const sf::Vector2u& size)
{
	_windowSize = size;
}

ObjectsManager::ObjectsManager()
{
	_textures = nullptr;
	_fonts = nullptr;
	_sounds = nullptr;
	_settings = Settings::GetInstance();

	_meleeWeapons["sword"] = nullptr;

	_hitboxWeapons["bite"] = nullptr;

	_enemies["devil"] = nullptr;

	_players["male_elf"] = nullptr;

	_focusContainers["option_bar"] = nullptr;

	_progressBars["heart"] = nullptr;

	_buttons["default_red"] = nullptr;

	_scenes["main_menu"] = nullptr;
	_scenes["options"] = nullptr;
	_scenes["game"] = nullptr;
}
ObjectsManager::~ObjectsManager()
{
	for (auto& v : _meleeWeapons)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _hitboxWeapons)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _enemies)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _players)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _focusContainers)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _progressBars)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _buttons)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _scenes)
		if (v.second != nullptr)
			delete v.second;
}


//Get
MeleeWeapon* ObjectsManager::GetMeleeWeapon(const std::string& name)
{
	auto found = _meleeWeapons.find(name);
	if (found != _meleeWeapons.end())
	{
		if (found->second == nullptr)
		{
			if(name == "sword") found->second = CreateMeleeWeaponSword();
		}

		auto obj = new MeleeWeapon(*(found->second));
		obj->GetTransformAnimation()->SetTarget(obj->GetAnimation()->ExternalTransform());
		return obj;
	}
	return new MeleeWeapon();
}

HitboxWeapon* ObjectsManager::GetHitboxWeapon(const std::string& name)
{
	auto found = _hitboxWeapons.find(name);
	if (found != _hitboxWeapons.end())
	{
		if (found->second == nullptr)
		{
			if (name == "bite") found->second = CreateHitboxWeaponBite();
		}

		auto obj = new HitboxWeapon(*(found->second));
		return obj;
	}
	return new HitboxWeapon();
}

Enemy* ObjectsManager::GetEnemy(const std::string& name)
{
	auto found = _enemies.find(name);
	if (found != _enemies.end())
	{
		if (found->second == nullptr)
		{
			if(name == "devil")found->second = CreateEnemyDevil();
		}

		auto obj = new Enemy(*(found->second));
		obj->GetAnimations()->UpdateCurrentAnimationPtr();
		return obj;
	}
	return new Enemy();
}

Player* ObjectsManager::GetPlayer(const std::string& name)
{
	auto found = _players.find(name);
	if (found != _players.end())
	{
		if (found->second == nullptr)
		{
			if (name == "male_elf")found->second = CreatePlayerMaleElf();
		}

		auto obj = new Player(*(found->second));
		obj->GetAnimations()->UpdateCurrentAnimationPtr();
		if (found->second->GetWeapon() != nullptr)
		{
			obj->SetWeaponUnsafe(found->second->GetWeapon()->clone());
			obj->GetWeapon()->GetTransformAnimation()->SetTarget(obj->GetWeapon()->GetAnimation()->ExternalTransform());
		}
		return obj;
	}
	return new Player();
}

FocusContainer* ObjectsManager::GetFocusContainer(const std::string& name)
{
	auto found = _focusContainers.find(name);
	if (found != _focusContainers.end())
	{
		if (found->second == nullptr)
		{
			if (name == "option_bar") found->second = CreateFocusContainerOptionBar();
		}

		FocusContainer* obj = new FocusContainer(*(found->second));
		obj->RedrawElement();
		return obj;
	}
	return nullptr;
}

ProgressBar* ObjectsManager::GetProgressBar(const std::string& name)
{
	auto found = _progressBars.find(name);
	if (found != _progressBars.end())
	{
		if (found->second == nullptr)
		{
			if (name == "heart") found->second = CreateProgressBarHeart();
		}

		ProgressBar* obj = new ProgressBar(*(found->second));
		obj->RedrawElement();
		return obj;
	}
	return nullptr;
}

Button* ObjectsManager::GetButton(const std::string& name)
{
	auto found = _buttons.find(name);
	if (found != _buttons.end())
	{
		if (found->second == nullptr)
		{
			if (name == "default_red") found->second = CreateButtonDefaultRed();
		}

		Button* obj = new Button(*(found->second));
		obj->RedrawElement();
		return obj;
	}
	return nullptr;
}

Scene* ObjectsManager::GetScene(const std::string& name)
{
	auto found = _scenes.find(name);
	if (found != _scenes.end())
	{
		if (found->second == nullptr)
		{
			if (name == "main_menu") found->second = CreateSceneMainMenu();
			else if (name == "options") found->second = CreateSceneOptions();
			else if (name == "game") found->second = CreateSceneGameUI();
		}
		Scene* obj = new Scene(*(found->second));
		obj->RefreshElements();
		return obj;
	}
	return nullptr;
}

//Create
MeleeWeapon* ObjectsManager::CreateMeleeWeaponSword()
{
	MeleeWeapon* sword = new MeleeWeapon();

	sf::Animation weapon;
	weapon.SetTexture(_textures->GetTexture("tiles1"));
	weapon.AddNewFrame(sf::IntRect(208, 32, 16, 32));
	weapon.setScale(0.6F, 0.6F);

	sword->SetAnimation(weapon);

	sf::TransformAnimation attack;
	attack.SetTarget(sword->GetAnimation()->ExternalTransform());

	sf::Transformable start;
	start.setRotation(-30);
	start.setOrigin(8, 42);
	attack.AddTransform(start, 0);

	sf::Transformable second;
	second.setRotation(-0.000000001F);
	second.setOrigin(8, 42);
	attack.AddTransform(second, 5);

	sf::Transformable third;
	third.setRotation(0);
	third.setOrigin(8, 42);
	attack.AddTransform(third, 0);

	sf::Transformable swing;
	swing.setRotation(30);
	swing.setOrigin(8, 42);
	attack.AddTransform(swing, 5);
	attack.SetLoop(false);

	sword->SetTransformAnimation(attack);

	sword->SetSoundsManager(_sounds);
	sword->GetSwingSounds()->push_back("weapons_swing1");
	sword->GetSwingSounds()->push_back("weapons_swing2");
	sword->GetSwingSounds()->push_back("weapons_swing3");

	return sword;
}

HitboxWeapon* ObjectsManager::CreateHitboxWeaponBite()
{
	HitboxWeapon* bite = new HitboxWeapon();

	bite->SetWeaponCooldown(60);
	bite->SetWeaponDMG(0.25F);
	bite->SetSoundsManager(_sounds);
	bite->GetHitSounds()->push_back("weapons_bite1");

	return bite;
}

Enemy* ObjectsManager::CreateEnemyDevil()
{
	Enemy* devil = new Enemy();

	//Graphics
	sf::Animation idle;
	idle.SetTexture(_textures->GetTexture("tiles2"));
	idle.AddNewFrame(TilesHelper::GetTileRect(idle.GetTexture()->getSize(), 16, 32, 344));
	idle.AddNewFrame(TilesHelper::GetTileRect(idle.GetTexture()->getSize(), 16, 32, 345));
	idle.SetChangeFrameEvery(15);

	sf::Animation move;
	move.SetTexture(_textures->GetTexture("tiles2"));
	move.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 347));
	move.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 348));
	move.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 349));
	move.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 350));
	move.SetChangeFrameEvery(3);

	sf::Animation attack;
	attack.SetTexture(_textures->GetTexture("tiles2"));
	attack.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 346));
	attack.AddNewFrame(TilesHelper::GetTileRect(move.GetTexture()->getSize(), 16, 32, 344));
	attack.SetChangeFrameEvery(20);

	sf::AnimationContainer container;
	container.SetStateAnimation("idle", idle);
	container.SetStateAnimation("move", move);
	container.SetStateAnimation("attack", attack);

	devil->SetAnimations(container);
	devil->SetState("idle");
	devil->SetScale(0.6F, 0.6F);
	devil->SetCollisionBoxOffset(sf::FloatRect(3.F, 17.F, 10.F, 12.F));
	devil->SetPosition(0.F, 0.F);

	//Sounds
	devil->SetSoundsManager(_sounds);
	devil->AddTakingDmgSound("entities_dmg4");
	devil->AddTakingDmgSound("entities_dmg5");
	devil->AddTakingDmgSound("entities_dmg6");

	//Pathfinding
	auto box = devil->GetCollisionBox();
	auto upperLeft = sf::Vector2f(box.left, box.top);
	auto center = sf::Vector2f(box.left + (box.width / 2), box.top + (box.height / 2));
	devil->SetAvoidanceRadius(MathHelper::GetDistanceBetweenPoints(upperLeft, center));

	//Stats
	devil->SetHealth(.5F);
	devil->SetSpeed(.5F);

	return devil;
}

Player* ObjectsManager::CreatePlayerMaleElf()
{
	Player* pl = new Player();

	//Graphics
	sf::AnimationContainer playerAnimations;

	sf::Animation idle;
	idle.SetTexture(_textures->GetTexture("players"));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 8));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 9));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 10));
	idle.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 11));
	idle.SetChangeFrameEvery(7);

	sf::Animation move;
	move.SetTexture(_textures->GetTexture("players"));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 12));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 13));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 14));
	move.AddNewFrame(TilesHelper::GetTileRect(_textures->GetTexture("players")->getSize(), 16, 22, 15));
	move.SetChangeFrameEvery(3);

	playerAnimations.SetStateAnimation("idle", idle);
	playerAnimations.SetStateAnimation("move", move);
	playerAnimations.ApplySetScale(1.f, 1.f);

	pl->SetAnimations(playerAnimations);
	pl->SetState("idle");
	pl->SetCollisionBoxOffset(sf::FloatRect(3.f, 6.f, 9.f, 15.f));
	pl->SetPosition(sf::Vector2f(296.f, 472.f));

	//Weapon
	pl->SetWeapon(GetMeleeWeapon("sword"));

	//Sounds
	pl->SetSoundsManager(_sounds);
	
	//Stats
	pl->SetSpeed(1.f);
	pl->SetHealth(3.f);
	pl->SetStep(2.f);

	return pl;
}

FocusContainer* ObjectsManager::CreateFocusContainerOptionBar()
{
	FocusContainer* obj = new FocusContainer();

	obj->SetFocusColor(sf::Color(0, 0, 0, 80));
	obj->SetHoverColor(sf::Color(0, 0, 0, 48));
	obj->SetKeyboardInput(true);
	obj->SetMouseInput(true);
	obj->SetPassHover(true);
	obj->SetPassClick(false);
	obj->setPosition(50.f, 50.f);

	//Elements
	auto label = new Label();
	auto pb = GetProgressBar("heart");

	label->SetFont(*_fonts->GetFont("menu"));
	label->SetText("Sound volume");
	label->SetCharacterSize(26);
	label->SetFillColor(sf::Color::White);
	label->SetMouseInput(false);
	label->SetKeyboardInput(false);
	label->setPosition(10.f, 10.f);
	auto bounds = label->GetGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(bounds.width + 10.f), uint32_t(bounds.height + 10.f)));

	pb->SetKeyboardInput(true);
	pb->SetMouseInput(true);
	pb->setPosition(_windowSize.x - 110.f - pb->GetGlobalBounds().width, 10.f);

	obj->AddElement("label", label);
	obj->AddElement("bar", pb);
	
	bounds = obj->GetElementsGlobalBounds();
	obj->Init(sf::Vector2u(uint32_t(bounds.width + 10.f), uint32_t(bounds.height + 10.f)));

	return obj;
}

ProgressBar* ObjectsManager::CreateProgressBarHeart()
{
	ProgressBar* hb = new ProgressBar();
	hb->SetCurrentValue(3.F);
	hb->SetMaxValue(3.f);
	hb->setPosition(sf::Vector2f(0.f, 0.f));

	hb->SetSoundsManager(_sounds);
	hb->SetTexturesManager(_textures);
	hb->AddProgressBarStep(sf::FloatRect(51.f, 134.f, 15.f, 15.f), "ui");
	hb->AddProgressBarStep(sf::FloatRect(35.f, 134.f, 15.f, 15.f), "ui");
	hb->AddProgressBarStep(sf::FloatRect(19.f, 134.f, 15.f, 15.f), "ui");

	hb->SetVisibility(true);
	hb->SetMouseInput(false);
	hb->SetKeyboardInput(false);

	hb->Init(sf::Vector2u(45, 15));
	hb->setScale(sf::Vector2f(4.f * _settings->SCALE_RATIO, 4.f * _settings->SCALE_RATIO));

	return hb;
}

Button* ObjectsManager::CreateButtonDefaultRed()
{
	Button* btn = new Button();

	//Init
	btn->Init(sf::Vector2u(uint32_t(170.f * _settings->SCALE_RATIO) + 1, uint32_t(96.f * _settings->SCALE_RATIO) + 1));
	btn->SetTexturesManager(_textures);
	btn->SetSoundsManager(_sounds);
	btn->SetBackgroundSize(sf::Vector2f(170.f * _settings->SCALE_RATIO, 96.f * _settings->SCALE_RATIO));
	btn->setPosition(0.f, 0.f);

	//text general
	sf::Text text;
	text.setFont(*_fonts->GetFont("menu"));
	text.setCharacterSize(uint32_t(17.f * _settings->SCALE_RATIO));
	text.setFillColor(sf::Color::White);
	text.setString("[default]");

	//none
	text.setPosition(roundf(5.f * _settings->SCALE_RATIO), roundf(36.f * _settings->SCALE_RATIO));
	btn->AddState("none", text, "ui", sf::FloatRect(15.f, 159.f, 34.f, 24.f));

	//hover
	text.setPosition(roundf(5.f * _settings->SCALE_RATIO), roundf(36.f * _settings->SCALE_RATIO));
	btn->AddState("hover", text, "ui", sf::FloatRect(15.f, 186.f, 34.f, 24.f));

	//click
	text.setPosition(roundf(10.f * _settings->SCALE_RATIO), roundf(40.f * _settings->SCALE_RATIO));
	text.setFillColor(sf::Color(230, 230, 230, 255));
	btn->AddState("click", text, "ui", sf::FloatRect(15.f, 210.f, 34.f, 24.f));

	return btn;
}

Scene* ObjectsManager::CreateSceneMainMenu()
{
	Scene* sc = new Scene();

	//Scene settings
	sc->SetBackgroundColor(sf::Color(66, 40, 53, 255));

	//AnimationBoxes
	auto leftCharacter = new AnimationBox();
	auto rightCharacter = new AnimationBox();

	//leftCharacter
	sf::Animation lc;
	lc.SetAnimationSpeed(1.f);
	lc.SetChangeFrameEvery(7);
	lc.SetLoop(true);
	lc.SetTexture(_textures->GetTexture("players"));
	lc.setPosition(0.f, 0.f);
	lc.setScale(12.8f * _settings->SCALE_RATIO, 12.8f * _settings->SCALE_RATIO);
	lc.AddNewFrame(sf::IntRect(0, 112, 14, 20));
	lc.AddNewFrame(sf::IntRect(16, 112, 14, 20));
	lc.AddNewFrame(sf::IntRect(32, 112, 14, 20));
	lc.AddNewFrame(sf::IntRect(48, 112, 14, 20));
	lc.Start();

	auto bounds = lc.GetGlobalBounds();
	leftCharacter->Init(sf::Vector2u((uint32_t)bounds.width, (uint32_t)bounds.height));
	leftCharacter->SetAnimation(lc);
	auto pos = ViewHelper::GetScaled(sf::FloatRect(0.2f, 0.6f, 1.f, 1.f), leftCharacter->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	leftCharacter->setPosition(pos.left, pos.top);


	//rightCharacter
	sf::Animation rc;
	rc.SetAnimationSpeed(1.f);
	rc.SetChangeFrameEvery(7);
	rc.SetLoop(true);
	rc.SetTexture(_textures->GetTexture("tiles2"));
	rc.setPosition(0.f, 0.f);
	rc.setScale(9.6f * _settings->SCALE_RATIO, 9.6f * _settings->SCALE_RATIO);
	rc.AddNewFrame(sf::IntRect(22, 277, 20, 27));
	rc.AddNewFrame(sf::IntRect(54, 277, 20, 27));
	rc.AddNewFrame(sf::IntRect(86, 277, 20, 27));
	rc.AddNewFrame(sf::IntRect(118, 277, 20, 27));
	rc.Start();

	bounds = rc.GetGlobalBounds();
	rightCharacter->Init(sf::Vector2u((uint32_t)bounds.width, (uint32_t)bounds.height));
	rightCharacter->SetAnimation(rc);
	pos = ViewHelper::GetScaled(sf::FloatRect(0.8f, 0.6f, 1.f, 1.f), rightCharacter->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	rightCharacter->setPosition(pos.left, pos.top);

	//Labels
	auto title = new Label();

	//title
	title->SetFont(*_fonts->GetFont("menu"));
	title->SetFillColor(sf::Color::White);
	title->SetText("Rogue Maze");
	title->SetCharacterSize(uint32_t(50.f * _settings->SCALE_RATIO));
	pos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.15f, 1.f, 1.f), title->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	title->setPosition(pos.left, pos.top);
	title->Init(sf::Vector2u((uint32_t)pos.width, (uint32_t)pos.height));

	//Buttons
	auto playButton = GetButton("default_red");
	auto optionsButton = GetButton("default_red");
	auto exitButton = GetButton("default_red");

	//playButton	
	playButton->ApplyText("Play");
	playButton->ApplyCharacterSize(uint32_t(24.f * _settings->SCALE_RATIO));
	playButton->EditTextState("none")->setPosition(roundf(33.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	playButton->EditTextState("hover")->setPosition(roundf(33.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	playButton->EditTextState("click")->setPosition(roundf(38.f * _settings->SCALE_RATIO), roundf(37.f * _settings->SCALE_RATIO));

	pos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.4f, 1.f, 1.f), playButton->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	playButton->setPosition(pos.left, pos.top);
	
	//optionsButton
	optionsButton->ApplyText("Options");
	optionsButton->ApplyCharacterSize(uint32_t(20.f * _settings->SCALE_RATIO));
	optionsButton->EditTextState("none")->setPosition(roundf(10.f * _settings->SCALE_RATIO), roundf(35.f * _settings->SCALE_RATIO));
	optionsButton->EditTextState("hover")->setPosition(roundf(10.f * _settings->SCALE_RATIO), roundf(35.f * _settings->SCALE_RATIO));
	optionsButton->EditTextState("click")->setPosition(roundf(15.f * _settings->SCALE_RATIO), roundf(40.f * _settings->SCALE_RATIO));

	pos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.6f, 1.f, 1.f), optionsButton->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	optionsButton->setPosition(pos.left, pos.top);
	
	//exitButton
	exitButton->ApplyText("Exit");
	exitButton->ApplyCharacterSize(uint32_t(24.f * _settings->SCALE_RATIO));
	exitButton->EditTextState("none")->setPosition(roundf(34.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	exitButton->EditTextState("hover")->setPosition(roundf(34.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	exitButton->EditTextState("click")->setPosition(roundf(39.f * _settings->SCALE_RATIO), roundf(37.f * _settings->SCALE_RATIO));

	pos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.8f, 1.f, 1.f), exitButton->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	exitButton->setPosition(pos.left, pos.top);

	//Add to scene
	sc->AddElement("leftCharacter", leftCharacter);
	sc->AddElement("rightCharacter", rightCharacter);
	sc->AddElement("title", title);
	sc->AddElement("play", playButton);
	sc->AddElement("options", optionsButton);
	sc->AddElement("exit", exitButton);

	return sc;
}
Scene* ObjectsManager::CreateSceneOptions()
{
	Scene* sc = new Scene();

	//Scene settings
	sc->SetBackgroundColor(sf::Color(66, 40, 53, 255));

	auto btn = GetFocusContainer("option_bar");
	sc->AddElement("btn", btn);

	return sc;
}
Scene* ObjectsManager::CreateSceneGameUI()
{
	Scene* sc = new Scene();

	//Scene settings
	sc->SetBackgroundColor(sf::Color(0, 0, 0, 0));

	//ProgressBars
	auto heart = GetProgressBar("heart");

	//heart
	heart->setPosition(0.f, 0.f);

	//Labels
	auto money = new Label();

	//money
	money->Init(sf::Vector2u(uint32_t(200.f * _settings->SCALE_RATIO) + 1, uint32_t(34.f * _settings->SCALE_RATIO) + 1));
	money->SetFont(*_fonts->GetFont("menu"));
	money->SetCharacterSize(uint32_t(32.f * _settings->SCALE_RATIO));
	money->SetText("000000");
	money->SetFillColor(sf::Color::White);
	money->setPosition((float)_windowSize.x - money->GetGlobalBounds().width - (8.f * _settings->SCALE_RATIO), 4.f * _settings->SCALE_RATIO);

	//AnimationBoxes
	auto coin = new AnimationBox();

	//coin
	sf::Animation c;
	c.SetAnimationSpeed(1.f);
	c.SetChangeFrameEvery(5);
	c.SetLoop(true);
	c.SetTexture(_textures->GetTexture("tiles2"));
	c.setPosition(0.f, 0.f);
	c.setScale(4.f * _settings->SCALE_RATIO, 4.f * _settings->SCALE_RATIO);
	c.AddNewFrame(sf::IntRect(289, 273, 6, 7));
	c.AddNewFrame(sf::IntRect(297, 273, 6, 7));
	c.AddNewFrame(sf::IntRect(305, 273, 6, 7));
	c.AddNewFrame(sf::IntRect(313, 273, 6, 7));
	c.Start();

	coin->Init(sf::Vector2u(uint32_t(24.f * _settings->SCALE_RATIO) + 1, uint32_t(28.f * _settings->SCALE_RATIO) + 1));
	coin->SetAnimation(c);
	coin->setPosition((float)_windowSize.x - money->GetGlobalBounds().width - (40.f * _settings->SCALE_RATIO), 4.f * _settings->SCALE_RATIO);

	//Add to scene
	sc->AddElement("healthBar", heart);
	sc->AddElement("coin", coin);
	sc->AddElement("money", money);

	return sc;
}

