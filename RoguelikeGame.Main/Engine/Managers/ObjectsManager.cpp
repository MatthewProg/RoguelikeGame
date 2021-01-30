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
}

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
