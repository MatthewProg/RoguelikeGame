#include "ObjectsManager.h"

void ObjectsManager::SetTexturesManager(TexturesManager* textures)
{
	_textures = textures;
}

ObjectsManager::ObjectsManager()
{
	_textures = nullptr;

	_meleeWeapons["sword"] = nullptr;

	_hitboxWeapons["bite"] = nullptr;

	_enemies["devil"] = nullptr;

	_progressBars["heart"] = nullptr;

	_players["male_elf"] = nullptr;
}
ObjectsManager::~ObjectsManager()
{
	for (auto v : _meleeWeapons)
		if (v.second != nullptr)
			delete v.second;

	for (auto v : _hitboxWeapons)
		if (v.second != nullptr)
			delete v.second;

	for (auto v : _enemies)
		if (v.second != nullptr)
			delete v.second;

	for (auto v : _progressBars)
		if (v.second != nullptr)
			delete v.second;

	for (auto v : _players)
		if (v.second != nullptr)
			delete v.second;
}


//Get
MeleeWeapon* ObjectsManager::GetMeleeWeapon(std::string name)
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

HitboxWeapon* ObjectsManager::GetHitboxWeapon(std::string name)
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

Enemy* ObjectsManager::GetEnemy(std::string name)
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

ProgressBar* ObjectsManager::GetProgressBar(std::string name)
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

Player* ObjectsManager::GetPlayer(std::string name)
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
		if (obj->GetWeapon() != nullptr)
		{
			switch (obj->GetWeapon()->GetWeaponType())
			{
			case WeaponType::NONE:
				obj->SetWeaponUnsafe(new HitboxWeapon(*((HitboxWeapon*)obj->GetWeapon())));
				break;
			case WeaponType::MELEE:
				obj->SetWeaponUnsafe(new MeleeWeapon(*((MeleeWeapon*)obj->GetWeapon())));
				break;
			default:
				obj->SetWeaponUnsafe(nullptr);
				break;
			}
			obj->GetWeapon()->GetTransformAnimation()->SetTarget(obj->GetWeapon()->GetAnimation()->ExternalTransform());
		}
		return obj;
	}
	return new Player();
}

//Create
MeleeWeapon* ObjectsManager::CreateMeleeWeaponSword()
{
	MeleeWeapon* sword = new MeleeWeapon();

	sf::Animation weapon;
	weapon.SetTexture(_textures->GetTexture("tiles1"));
	weapon.AddNewFrame(sf::IntRect(208, 32, 16, 32));
	weapon.setScale(0.6F, 0.6F);
	//weapon.setOrigin(8, 32);

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

	return sword;
}

HitboxWeapon* ObjectsManager::CreateHitboxWeaponBite()
{
	HitboxWeapon* bite = new HitboxWeapon();

	bite->SetWeaponCooldown(60);
	bite->SetWeaponDMG(0.25F);

	return bite;
}

Enemy* ObjectsManager::CreateEnemyDevil()
{
	Enemy* devil = new Enemy();

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

	auto box = devil->GetCollisionBox();
	auto upperLeft = sf::Vector2f(box.left, box.top);
	auto center = sf::Vector2f(box.left + (box.width / 2), box.top + (box.height / 2));
	devil->SetAvoidanceRadius(MathHelper::GetDistanceBetweenPoints(upperLeft, center));

	//Stats
	devil->SetHealth(.5F);
	devil->SetSpeed(.5F);

	return devil;
}

ProgressBar* ObjectsManager::CreateProgressBarHeart()
{
	ProgressBar* hb = new ProgressBar();
	hb->SetCurrentValue(3.F);
	hb->SetMaxValue(3.f);
	hb->setPosition(sf::Vector2f(0, 0));

	hb->SetTexturesManager(_textures);
	hb->AddProgressBarStep(sf::FloatRect(51, 134, 15, 15), "ui");
	hb->AddProgressBarStep(sf::FloatRect(35, 134, 15, 15), "ui");
	hb->AddProgressBarStep(sf::FloatRect(19, 134, 15, 15), "ui");

	hb->SetVisibility(true);
	hb->SetMouseInput(false);
	hb->SetKeyboardInput(false);

	hb->Init(sf::Vector2u(45, 15));
	hb->setScale(sf::Vector2f(4.f, 4.f));

	return hb;
}

Player* ObjectsManager::CreatePlayerMaleElf()
{
	Player* pl = new Player();

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
	playerAnimations.ApplySetScale(1, 1);

	pl->SetAnimations(playerAnimations);
	pl->SetState("idle");
	pl->SetCollisionBoxOffset(sf::FloatRect(3, 6, 9, 15));
	pl->SetPosition(sf::Vector2f(296, 472));
	pl->SetWeapon(GetMeleeWeapon("sword"));

	return pl;
}