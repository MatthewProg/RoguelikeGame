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
	_focusContainers["option_checkbox"] = nullptr;
	_focusContainers["option_listselect"] = nullptr;
	_focusContainers["option_text"] = nullptr;

	_progressBars["heart"] = nullptr;
	_progressBars["options"] = nullptr;

	_listSelects["options"] = nullptr;

	_scrollBars["default"] = nullptr;

	_checkBoxes["default"] = nullptr;

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

	for (auto& v : _listSelects)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _scrollBars)
		if (v.second != nullptr)
			delete v.second;

	for (auto& v : _checkBoxes)
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
			else if (name == "option_checkbox") found->second = CreateFocusContainerOptionCheckBox();
			else if (name == "option_listselect") found->second = CreateFocusContainerOptionListSelect();
			else if (name == "option_text") found->second = CreateFocusContainerOptionText();
		}

		FocusContainer* obj = new FocusContainer(*(found->second));
		obj->ForceRedraw();
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
			else if (name == "options") found->second = CreateProgressBarOptions();
		}

		ProgressBar* obj = new ProgressBar(*(found->second));
		obj->ForceRedraw();
		return obj;
	}
	return nullptr;
}

ListSelect* ObjectsManager::GetListSelect(const std::string& name)
{
	auto found = _listSelects.find(name);
	if (found != _listSelects.end())
	{
		if (found->second == nullptr)
		{
			if (name == "options") found->second = CreateListSelectOptions();
		}

		ListSelect* obj = new ListSelect(*(found->second));
		obj->ForceRedraw();
		return obj;
	}
	return nullptr;
}

ScrollBar* ObjectsManager::GetScrollBar(const std::string& name)
{
	auto found = _scrollBars.find(name);
	if (found != _scrollBars.end())
	{
		if (found->second == nullptr)
		{
			if (name == "default") found->second = CreateScrollBarDefault();
		}

		ScrollBar* obj = new ScrollBar(*(found->second));
		obj->ForceRedraw();
		return obj;
	}
	return nullptr;
}

CheckBox* ObjectsManager::GetCheckBox(const std::string& name)
{
	auto found = _checkBoxes.find(name);
	if (found != _checkBoxes.end())
	{
		if (found->second == nullptr)
		{
			if (name == "default") found->second = CreateCheckBoxDefault();
		}

		CheckBox* obj = new CheckBox(*(found->second));
		obj->ForceRedraw();
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
		obj->ForceRedraw();
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
	obj->setPosition(0.f, 0.f);

	//Elements
	auto label = new Label();
	auto pb = GetProgressBar("options");

	label->SetFont(*_fonts->GetFont("menu"));
	label->SetText("Sound volume");
	label->SetCharacterSize(uint32_t(float(26) * _settings->SCALE_RATIO));
	label->SetFillColor(sf::Color::White);
	label->SetMouseInput(false);
	label->SetKeyboardInput(false);
	label->setPosition(10.f * _settings->SCALE_RATIO, 10.f * _settings->SCALE_RATIO);
	auto bounds = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	pb->SetKeyboardInput(true);
	pb->SetMouseInput(true);
	pb->setPosition(_windowSize.x - (110.f * _settings->SCALE_RATIO) - pb->GetGlobalBounds().width, 20.f * _settings->SCALE_RATIO);

	obj->AddElement("label", label);
	obj->AddElement("bar", pb);
	
	bounds = obj->GetElementsGlobalBounds();
	auto labelPos = ViewHelper::GetScaled(sf::FloatRect(0.05f, 0.5f, 1.f, 1.f), label->GetTextGlobalBounds(), bounds);
	auto pbPos = ViewHelper::GetScaled(sf::FloatRect(0.9f, 0.5f, 1.f, 1.f), pb->GetGlobalBounds(), bounds);
	label->setPosition(10.f * _settings->SCALE_RATIO, labelPos.top);
	pb->setPosition(bounds.left + bounds.width - pb->GetGlobalBounds().width - (10.f * _settings->SCALE_RATIO), pbPos.top);
	obj->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	return obj;
}
FocusContainer* ObjectsManager::CreateFocusContainerOptionCheckBox()
{
	FocusContainer* obj = new FocusContainer();

	obj->SetFocusColor(sf::Color(0, 0, 0, 80));
	obj->SetHoverColor(sf::Color(0, 0, 0, 48));
	obj->SetKeyboardInput(true);
	obj->SetMouseInput(true);
	obj->SetPassHover(true);
	obj->SetPassClick(false);
	obj->setPosition(0.f, 0.f);

	//Elements
	auto label = new Label();
	auto cb = GetCheckBox("default");

	label->SetFont(*_fonts->GetFont("menu"));
	label->SetText("V-Sync");
	label->SetCharacterSize(uint32_t(float(26) * _settings->SCALE_RATIO));
	label->SetFillColor(sf::Color::White);
	label->SetMouseInput(false);
	label->SetKeyboardInput(false);
	label->setPosition(10.f * _settings->SCALE_RATIO, 10.f * _settings->SCALE_RATIO);
	auto bounds = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	cb->SetKeyboardInput(true);
	cb->SetMouseInput(true);
	cb->setPosition(_windowSize.x - (110.f * _settings->SCALE_RATIO) - cb->GetGlobalBounds().width, 20.f * _settings->SCALE_RATIO);

	obj->AddElement("label", label);
	obj->AddElement("checkbox", cb);

	bounds = obj->GetElementsGlobalBounds();
	auto labelPos = ViewHelper::GetScaled(sf::FloatRect(0.05f, 0.5f, 1.f, 1.f), label->GetTextGlobalBounds(), bounds);
	auto cbPos = ViewHelper::GetScaled(sf::FloatRect(0.9f, 0.5f, 1.f, 1.f), cb->GetGlobalBounds(), bounds);
	label->setPosition(10.f * _settings->SCALE_RATIO, labelPos.top);
	cb->setPosition(bounds.left + bounds.width - cb->GetGlobalBounds().width - (10.f * _settings->SCALE_RATIO), cbPos.top);
	obj->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	return obj;
}

FocusContainer* ObjectsManager::CreateFocusContainerOptionListSelect()
{
	FocusContainer* obj = new FocusContainer();

	obj->SetFocusColor(sf::Color(0, 0, 0, 80));
	obj->SetHoverColor(sf::Color(0, 0, 0, 48));
	obj->SetKeyboardInput(true);
	obj->SetMouseInput(true);
	obj->SetPassHover(true);
	obj->SetPassClick(false);
	obj->setPosition(0.f, 0.f);

	//Elements
	auto label = new Label();
	auto ls = GetListSelect("options");

	label->SetFont(*_fonts->GetFont("menu"));
	label->SetText("Display mode");
	label->SetCharacterSize(uint32_t(float(26) * _settings->SCALE_RATIO));
	label->SetFillColor(sf::Color::White);
	label->SetMouseInput(false);
	label->SetKeyboardInput(false);
	label->setPosition(10.f * _settings->SCALE_RATIO, 10.f * _settings->SCALE_RATIO);
	auto bounds = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	ls->setPosition(_windowSize.x - (110.f * _settings->SCALE_RATIO) - ls->GetGlobalBounds().width, 20.f * _settings->SCALE_RATIO);

	obj->AddElement("label", label);
	obj->AddElement("listselect", ls);

	bounds = obj->GetElementsGlobalBounds();
	auto labelPos = ViewHelper::GetScaled(sf::FloatRect(0.05f, 0.5f, 1.f, 1.f), label->GetTextGlobalBounds(), bounds);
	auto lsPos = ViewHelper::GetScaled(sf::FloatRect(0.9f, 0.5f, 1.f, 1.f), ls->GetGlobalBounds(), bounds);
	label->setPosition(10.f * _settings->SCALE_RATIO, labelPos.top);
	ls->setPosition(bounds.left + bounds.width - ls->GetGlobalBounds().width - (10.f * _settings->SCALE_RATIO), lsPos.top);
	obj->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	return obj;
}

FocusContainer* ObjectsManager::CreateFocusContainerOptionText()
{
	FocusContainer* obj = new FocusContainer();

	obj->SetFocusColor(sf::Color(0, 0, 0, 80));
	obj->SetHoverColor(sf::Color(0, 0, 0, 48));
	obj->SetKeyboardInput(true);
	obj->SetMouseInput(true);
	obj->SetPassHover(true);
	obj->SetPassClick(false);
	obj->setPosition(0.f, 0.f);

	//Elements
	auto label = new Label();
	auto button = new Button();

	label->SetFont(*_fonts->GetFont("menu"));
	label->SetText("Move left");
	label->SetCharacterSize(uint32_t(float(26) * _settings->SCALE_RATIO));
	label->SetFillColor(sf::Color::White);
	label->SetMouseInput(false);
	label->SetKeyboardInput(false);
	label->setPosition(10.f * _settings->SCALE_RATIO, 10.f * _settings->SCALE_RATIO);
	auto bounds = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

	button->SetSoundsManager(_sounds);
	button->SetTexturesManager(_textures);
	button->SetKeyboardInput(true);
	button->SetMouseInput(true);
	button->SetBackgroundSize(sf::Vector2f(0.f, 0.f));
	sf::Text t;
	t.setFont(*_fonts->GetFont("menu"));
	t.setString("Left");
	t.setCharacterSize(uint32_t(float(26) * _settings->SCALE_RATIO));
	t.setFillColor(sf::Color::White);
	button->AddState("none", t, "", sf::FloatRect(0.f, 0.f, 0.f, 0.f));
	button->AddState("hover", t, "", sf::FloatRect(0.f, 0.f, 0.f, 0.f));
	button->AddState("click", t, "", sf::FloatRect(0.f, 0.f, 0.f, 0.f));
	button->SetHoverSound("");
	button->SetPressSound("");
	button->SetReleaseSound("");
	button->SetBackgroundSize(sf::Vector2f(0.f, 0.f));
	bounds = t.getGlobalBounds();
	button->Init(sf::Vector2u(uint32_t(ceilf(bounds.left + bounds.width)), uint32_t(ceilf(bounds.top + bounds.height))));
	button->setPosition(_windowSize.x - (110.f * _settings->SCALE_RATIO) - button->GetGlobalBounds().width, 20.f * _settings->SCALE_RATIO);

	obj->AddElement("label", label);
	obj->AddElement("text", button);

	bounds = obj->GetElementsGlobalBounds();
	auto labelPos = ViewHelper::GetScaled(sf::FloatRect(0.05f, 0.5f, 1.f, 1.f), label->GetTextGlobalBounds(), bounds);
	auto bPos = ViewHelper::GetScaled(sf::FloatRect(0.9f, 0.5f, 1.f, 1.f), button->GetGlobalBounds(), bounds);
	label->setPosition(10.f * _settings->SCALE_RATIO, labelPos.top);
	button->setPosition(bounds.left + bounds.width - button->GetGlobalBounds().width - (10.f * _settings->SCALE_RATIO), bPos.top);
	obj->Init(sf::Vector2u(uint32_t(bounds.width), uint32_t(bounds.height)));

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
ProgressBar* ObjectsManager::CreateProgressBarOptions()
{
	ProgressBar* pb = new ProgressBar();

	pb->SetCurrentValue(100.F);
	pb->SetMaxValue(100.f);
	pb->SetForceStep(10.f);
	pb->setPosition(sf::Vector2f(0.f, 0.f));
	
	pb->SetSoundsManager(_sounds);
	pb->SetTexturesManager(_textures);
	pb->AddBackgroundLayer(sf::FloatRect(11.f, 120.f, 49.f, 10.f), "ui");
	pb->AddProgressBarStep(sf::FloatRect(0.f, 0.f, 4.f, 6.f), "ui");
	pb->AddProgressBarStep(sf::FloatRect(42.f, 15.f, 4.f, 6.f), "ui");
	pb->SetProgressBarStepsPos(sf::Vector2f(4.f, 2.f));
	
	pb->SetVisibility(true);
	pb->SetMouseInput(true);
	pb->SetKeyboardInput(true);
	
	pb->Init(sf::Vector2u(49, 10));
	pb->setScale(sf::Vector2f(4.f * _settings->SCALE_RATIO, 4.f * _settings->SCALE_RATIO));

	return pb;
}

ListSelect* ObjectsManager::CreateListSelectOptions()
{
	ListSelect* ls = new ListSelect(_sounds, _textures);

	ls->Init(sf::Vector2u(250, 29));
	ls->SetMouseInput(true);
	ls->SetKeyboardInput(true);

	ls->SetLeftArrowSize(sf::Vector2f(16.f, 29.f));
	ls->AddLeftArrowState("none", "ui", sf::FloatRect(110.f, 74.f, 11.f, 20.f));
	ls->AddLeftArrowState("hover", "ui", sf::FloatRect(121.f, 74.f, 11.f, 20.f));
	ls->AddLeftArrowState("click", "ui", sf::FloatRect(132.0f, 74.f, 11.f, 20.f));
	ls->TransformLeftArrow()->setPosition(0.f, 0.f);

	ls->SetRightArrowSize(sf::Vector2f(16.f, 29.f));
	ls->AddRightArrowState("none", "ui", sf::FloatRect(110.f, 94.f, 11.f, 20.f));
	ls->AddRightArrowState("hover", "ui", sf::FloatRect(121.f, 94.f, 11.f, 20.f));
	ls->AddRightArrowState("click", "ui", sf::FloatRect(132.0f, 94.f, 11.f, 20.f));
	ls->TransformRightArrow()->setPosition(234.f, 0.f);

	ls->SetTextSize(sf::Vector2f(208.f, 20.f));
	ls->SetFont(*_fonts->GetFont("menu"));
	ls->SetCharacterSize(20);
	ls->TransformText()->setPosition(21.f, 4.f);

	ls->AddValue("Windowed");
	ls->AddValue("Borderless");
	ls->AddValue("Fullscreen");
	ls->SetCurrentIndex(0);
	ls->SetHorizontalAlignment(UIElement::Align::MIDDLE);

	ls->setPosition(150.f, 400.f);
	ls->setScale(_settings->SCALE_RATIO, _settings->SCALE_RATIO);
	return ls;
}

ScrollBar* ObjectsManager::CreateScrollBarDefault()
{
	ScrollBar* sb = new ScrollBar(_sounds, _textures);

	sb->setPosition(10.f, 10.f);
	sb->setRotation(-90.f);
	sb->setOrigin(163.f, 0.f);
	sb->setScale(3.f * _settings->SCALE_RATIO, 3.f * _settings->SCALE_RATIO);
	sb->Init(sf::Vector2u(163, 11));

	sb->SetMouseInput(true);
	sb->SetKeyboardInput(true);

	sb->SetHorizontalSource(true);
	sb->SetHorizontalInput(false);
	sb->SetTrackLength(150.f);
	sb->SetScroll(0.5f);
	sb->SetScrollAmount(0.05f);

	sb->SetTrackTexture("default_track", "ui", sf::FloatRect(134.f, 136.f, 10.f, 9.f));
	sb->SetBeginningTexture("ui", sf::FloatRect(128.f, 136.f, 5.f, 9.f));
	sb->SetEndingTexture("ui", sf::FloatRect(145.f, 136.f, 6.f, 9.f));
	sb->ThumbAddState("none", "ui", sf::FloatRect(100.f, 135.f, 13.f, 11.f));
	sb->ThumbAddState("hover", "ui", sf::FloatRect(86.f, 135.f, 13.f, 11.f));
	sb->ThumbAddState("click", "ui", sf::FloatRect(114.f, 135.f, 13.f, 11.f));
	sb->SetThumbBackgroundSize(sf::Vector2f(13.f, 11.f));
	sb->SetTrackOffset(sf::Vector2f(1.f, 1.f));

	return sb;
}

CheckBox* ObjectsManager::CreateCheckBoxDefault()
{
	CheckBox* cb = new CheckBox();

	//Init
	cb->Init(sf::Vector2u(24, 24));
	cb->SetTexturesManager(_textures);
	cb->SetSoundsManager(_sounds);
	cb->setPosition(0.f, 0.f);
	cb->setScale(2.f * _settings->SCALE_RATIO, 2.f * _settings->SCALE_RATIO);

	cb->SetTextureName("ui");
	cb->SetUncheckedRect(sf::FloatRect(108.f, 40.f, 24.f, 24.f));
	cb->SetCheckedRect(sf::FloatRect(134.f, 40.f, 24.f, 24.f));
	cb->SetChecked(false);

	return cb;
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
	pos = ViewHelper::GetScaled(sf::FloatRect(0.5f, 0.15f, 1.f, 1.f), title->GetTextGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
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

	//Vars
	auto sounds = GetFocusContainer("option_bar");
	auto music = GetFocusContainer("option_bar");
	auto vsync = GetFocusContainer("option_checkbox");
	auto displayMode = GetFocusContainer("option_listselect");
	auto antialiasing = GetFocusContainer("option_listselect");
	auto resolution = GetFocusContainer("option_listselect");
	auto fps = GetFocusContainer("option_listselect");
	auto mLeft = GetFocusContainer("option_text");
	auto mRight = GetFocusContainer("option_text");
	auto mUp = GetFocusContainer("option_text");
	auto mDown = GetFocusContainer("option_text");
	auto scrollBar = GetScrollBar("default");
	auto scrollView = new ScrollView();
	auto saveBtn = GetButton("default_red");

	//Sounds
	auto label = ((Label*)sounds->GetElement("label"));
	label->SetText("Sound volume");
	auto labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	sounds->Init(sf::Vector2u(914, 70));
	sounds->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Music
	label = ((Label*)music->GetElement("label"));
	label->SetText("Music volume");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	music->Init(sf::Vector2u(914, 70));
	music->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Vsync
	label = ((Label*)vsync->GetElement("label"));
	label->SetText("V-Sync");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	vsync->Init(sf::Vector2u(914, 70));
	vsync->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Display mod
	label = ((Label*)displayMode->GetElement("label"));
	label->SetText("Display mode");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	displayMode->Init(sf::Vector2u(914, 70));
	displayMode->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Antialiasing
	label = ((Label*)antialiasing->GetElement("label"));
	label->SetText("Antialiasing level");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	auto antiLS = ((ListSelect*)antialiasing->GetElement("listselect"));
	antiLS->ClearValues();
	antiLS->AddValue("0");
	antiLS->AddValue("2");
	antiLS->AddValue("4");
	antiLS->AddValue("8");
	antiLS->AddValue("16");
	antialiasing->Init(sf::Vector2u(914, 70));
	antialiasing->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Resolution
	label = ((Label*)resolution->GetElement("label"));
	label->SetText("Resolution");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	auto resLS = ((ListSelect*)resolution->GetElement("listselect"));
	resLS->ClearValues();
	auto desktopMode = sf::VideoMode::getDesktopMode();
	auto resVal = UIHelper::GetAllTypicalResolutions(desktopMode.width, desktopMode.height);
	for (auto& r : resVal)
		resLS->AddValue(std::to_string(r.x) + "x" + std::to_string(r.y));
	resolution->Init(sf::Vector2u(914, 70));
	resolution->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//FPS
	label = ((Label*)fps->GetElement("label"));
	label->SetText("Limit FPS");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	auto fpsLS = ((ListSelect*)fps->GetElement("listselect"));
	fpsLS->ClearValues();
	fpsLS->AddValue("25");
	fpsLS->AddValue("29");
	fpsLS->AddValue("30");
	fpsLS->AddValue("50");
	fpsLS->AddValue("59");
	fpsLS->AddValue("60");
	fpsLS->AddValue("75");
	fpsLS->AddValue("120");
	fpsLS->AddValue("144");
	fpsLS->AddValue("240");
	fps->Init(sf::Vector2u(914, 70));
	fps->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Move left
	label = ((Label*)mLeft->GetElement("label"));
	label->SetText("Move left");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	((Button*)mLeft->GetElement("text"))->ApplyText("Left");
	mLeft->Init(sf::Vector2u(914, 70));
	mLeft->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Move right
	label = ((Label*)mRight->GetElement("label"));
	label->SetText("Move right");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	auto mrButton = ((Button*)mRight->GetElement("text"));
	mrButton->ApplyText("Right");
	auto btnBounds = mrButton->EditTextState("none")->getGlobalBounds();
	mrButton->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
	mRight->Init(sf::Vector2u(914, 70));
	mrButton->setPosition(914.f - btnBounds.width - (10.f * _settings->SCALE_RATIO), 0.f);
	mRight->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Move up
	label = ((Label*)mUp->GetElement("label"));
	label->SetText("Move up");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	mrButton = ((Button*)mUp->GetElement("text"));
	mrButton->ApplyText("Up");
	btnBounds = mrButton->EditTextState("none")->getGlobalBounds();
	mrButton->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
	mUp->Init(sf::Vector2u(914, 70));
	mrButton->setPosition(914.f - btnBounds.width - (10.f * _settings->SCALE_RATIO), 0.f);
	mUp->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//Move down
	label = ((Label*)mDown->GetElement("label"));
	label->SetText("Move down");
	labelB = label->GetTextGlobalBounds();
	label->Init(sf::Vector2u(uint32_t(ceilf(labelB.width)), uint32_t(ceilf(labelB.height))));
	mrButton = ((Button*)mDown->GetElement("text"));
	mrButton->ApplyText("Down");
	btnBounds = mrButton->EditTextState("none")->getGlobalBounds();
	mrButton->Init(sf::Vector2u(uint32_t(ceilf(btnBounds.left + btnBounds.width)), uint32_t(ceilf(btnBounds.top + btnBounds.height))));
	mDown->Init(sf::Vector2u(914, 70));
	mrButton->setPosition(914.f - btnBounds.width - (10.f * _settings->SCALE_RATIO), 0.f);
	mDown->AutoAlignElementsVertically(UIElement::Align::MIDDLE);

	//ScrollBar
	scrollBar->setPosition(934.f, 0.f);
	scrollBar->setOrigin(150.f, 0.f);
	scrollBar->Init(sf::Vector2u(150, 11));
	scrollBar->SetTrackLength(137.f);
	scrollBar->SetScroll(1.f);

	//ScrollView
	scrollView->Init(sf::Vector2u(967, 450));
	scrollView->setPosition(18.5f, 20.f);
	scrollView->SetScrollBar(scrollBar);
	scrollView->SetListSize(sf::Vector2f(914.f, 450.f));
	scrollView->SetScrollHorizontaly(false);
	scrollView->AddElement("display_mode", displayMode);
	scrollView->AddElement("resolution", resolution);
	scrollView->AddElement("vsync", vsync);
	scrollView->AddElement("fps", fps);
	scrollView->AddElement("antialiasing", antialiasing);
	scrollView->AddElement("sound_volume", sounds);
	scrollView->AddElement("music_volume", music);
	scrollView->AddElement("move_left", mLeft);
	scrollView->AddElement("move_right", mRight);
	scrollView->AddElement("move_up", mUp);
	scrollView->AddElement("move_down", mDown);

	//Save button
	saveBtn->ApplyText("Save");
	saveBtn->ApplyCharacterSize(uint32_t(24.f * _settings->SCALE_RATIO));
	saveBtn->EditTextState("none")->setPosition(roundf(33.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	saveBtn->EditTextState("hover")->setPosition(roundf(33.f * _settings->SCALE_RATIO), roundf(33.f * _settings->SCALE_RATIO));
	saveBtn->EditTextState("click")->setPosition(roundf(38.f * _settings->SCALE_RATIO), roundf(37.f * _settings->SCALE_RATIO));

	auto pos = ViewHelper::GetScaled(sf::FloatRect(0.93f, 0.92f, 1.f, 1.f), saveBtn->GetGlobalBounds(), sf::FloatRect(0.f, 0.f, (float)_windowSize.x, (float)_windowSize.y));
	saveBtn->setPosition(pos.left, pos.top);
	saveBtn->setScale(0.75f, 0.75f);

	//Add
	sc->AddElement("view", scrollView);
	sc->AddElement("save_button", saveBtn);
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
	money->setPosition((float)_windowSize.x - money->GetTextGlobalBounds().width - (8.f * _settings->SCALE_RATIO), 4.f * _settings->SCALE_RATIO);

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
	coin->setPosition((float)_windowSize.x - money->GetTextGlobalBounds().width - (40.f * _settings->SCALE_RATIO), 4.f * _settings->SCALE_RATIO);

	//Add to scene
	sc->AddElement("healthBar", heart);
	sc->AddElement("coin", coin);
	sc->AddElement("money", money);

	return sc;
}

