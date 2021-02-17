#include "EnemiesManager.h"

EnemiesManager::EnemiesManager()
{
	_enemies.clear();
	_logger = Logger::GetInstance();
	_player = nullptr;
}

EnemiesManager::~EnemiesManager()
{
	for (auto it : _enemies)
		if(it != nullptr)
			delete it;
}

void EnemiesManager::Update(bool tick, float deltaTime)
{
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		auto it = _enemies[i];

		if (tick) //No need to do it every frame
		{
			if (it->IsDead())
			{
				_enemies.erase(_enemies.begin() + i);
				continue;
			}
		}

		it->Update(tick, deltaTime);

		auto wpn = it->GetWeapon();
		if (wpn == nullptr) continue;

		wpn->setPosition(ViewHelper::GetRectCenter(it->GetCollisionBox()));
	}
}

void EnemiesManager::CheckForHit()
{
	//Player -> Enemy
	auto weapon = _player->GetWeapon();
	auto playerCenter = ViewHelper::GetRectCenter(_player->GetCollisionBox());
	auto playerView = _player->GetView();

	for (auto obj : _enemies)
	{
		if (CollisionHelper::CheckSimpleCollision(playerView, obj->GetCollisionBox()) == false) continue; //Not in player view
		if (weapon == nullptr) continue;

		if (weapon->GetWeaponType() == WeaponType::MELEE)
		{
			auto enemyHitboxPoints = CollisionHelper::GetRectPoints(obj->GetCollisionBox());
			MeleeWeapon* wep = (MeleeWeapon*)weapon;
			for (auto p : enemyHitboxPoints)
			{
				if (CollisionHelper::CheckCircleCollision(p, playerCenter, wep->GetWeaponRange(), wep->GetWeaponAngle(), wep->GetCurrentAngle()))
				{
					obj->TakeDmg(wep->GetWeaponDMG());
					break;
				}
			}
		}
	}
}

void EnemiesManager::CheckAttacks()
{
	//Enemy -> Player
	auto playerHitbox = _player->GetCollisionBox();
	auto playerView = _player->GetView();

	for (auto enemy : _enemies)
	{
		if (CollisionHelper::CheckSimpleCollision(playerView, enemy->GetCollisionBox()) == false) continue; //Not in player view

		auto enemyWeapon = enemy->GetWeapon();
		if (enemyWeapon == nullptr) continue;

		if (enemyWeapon->CanAttack() == false)
			continue;

		if (enemyWeapon->GetWeaponType() == WeaponType::NONE) //Enemy attacks if hitboxes collide
		{
			auto enemyHitbox = enemy->GetCollisionBox();
			if (CollisionHelper::CheckSimpleCollision(enemyHitbox, playerHitbox))
			{
				_player->TakeDmg(enemyWeapon->GetWeaponDMG());
				enemy->Attack();
				continue;
			}
		}
	}
}

void EnemiesManager::SetPlayer(Player* player)
{
	_player = player;
}

void EnemiesManager::SetEnemiesHitboxVisibility(bool visibility)
{
	for (auto it : _enemies)
		it->SetHitboxVisibility(visibility);
}

bool EnemiesManager::GetEnemiesHitboxVisibility()
{
	if(_enemies.size() > 0)
		return _enemies[0]->GetHitboxVisibility();
	return false;
}

void EnemiesManager::ToggleEnemiesHitboxVisibility()
{
	std::string status = (!GetEnemiesHitboxVisibility()) ? "true" : "false";
	_logger->Log(Logger::LogType::INFO, "Show enemies hitbox: " + status);
	SetEnemiesHitboxVisibility(!GetEnemiesHitboxVisibility());
}

void EnemiesManager::ToggleEnemiesRaycastVisibility()
{
	bool status = false;
	bool logged = false;

	for (auto& enemy : _enemies)
	{
		auto enemyWeapon = enemy->GetWeapon();
		if (enemyWeapon != nullptr)
		{
			if (logged == false)
			{
				status = enemyWeapon->GetRaycastVisibility();
				std::string mess = (!status) ? "true" : "false";
				_logger->Log(Logger::LogType::INFO, "Show enemies weapon raycast: " + mess);
				logged = true;
			}
			enemyWeapon->SetRaycastVisibility(!status);
		}
	}
}

void EnemiesManager::Add(Enemy* enemy)
{
	_enemies.push_back(enemy);
}

std::vector<Enemy*>* EnemiesManager::GetEnemies()
{
	return &_enemies;
}

void EnemiesManager::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	for (auto it : _enemies)
		target.draw(*it);
}
