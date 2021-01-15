#pragma once

#include "../Managers/PathfindingManager.h"
#include "../Managers/CollisionsManager.h"
#include "../Managers/EnemiesManager.h"
#include "../Helpers/ViewHelper.h"

class EnemiesAI : public sf::Drawable
{
private:
	Logger* _logger;

	Entity* _target;
	EnemiesManager* _enemies;
	CollisionsManager* _collisions;
	PathfindingManager _pathfind;

	sf::Vector2f _lastTargetPos;
	std::map<Enemy*, std::vector<sf::Vector2f>> _enemyPath;

	sf::VertexArray _pathfindLines;
	sf::Color _pathfindLinesColor;
	bool _showPathfindLines;

	bool DirectLineOfSight(Enemy* source, sf::Vector2f* raycastHitpoint);
	void PrepareVertex();

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	EnemiesAI();
	~EnemiesAI();

	void Update(float deltaTime);
	void ClearEnemiesPaths();
	void MoveStraightToPoint(Enemy* source, sf::Vector2f point, float deltaTime);

	//EnemiesAI setters
	void SetPathfindVisibility(bool visible);
	void SetPathfindColor(sf::Color color);

	//EnemiesAI getters
	bool GetPathfindVisibility();
	sf::Color GetPathfindColor();

	void TogglePathfindingVisibility();

	void SetTarget(Entity* target);
	void SetEnemiesManager(EnemiesManager* manager);
	void SetCollisionsManager(CollisionsManager* manager);
	void SetPathfindPoints(std::vector<sf::Vector2f> points);

};

