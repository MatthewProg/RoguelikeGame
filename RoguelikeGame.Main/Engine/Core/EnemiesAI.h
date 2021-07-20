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
	std::unordered_map<Vector2MapKey<float>, bool, Vector2MapKeyHasher<float>> _lastNeighbours;
	std::map<Enemy*, std::list<sf::Vector2f>> _enemyPath;
	Paths _allPaths;

	sf::VertexArray _pathfindLines;
	sf::Color _pathfindLinesColor;
	bool _showPathfindLines;

	bool DirectLineOfSight(Enemy* source, sf::Vector2f& raycastHitpoint);
	void PrepareVertex();
	float WeightFunction(float x);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates) const override;
public:
	EnemiesAI();
	~EnemiesAI() = default;

	void Update(float deltaTime);
	void ClearEnemiesPaths();
	void MoveStraightToPoint(Enemy* source, const sf::Vector2f& point, float deltaTime);
	float GetBestAngle(float gotoAngle, const std::vector<float>& avoidAngle, uint8_t precision = 8);

	//EnemiesAI setters
	void SetPathfindVisibility(bool visible);
	void SetPathfindColor(const sf::Color& color);

	//EnemiesAI getters
	bool GetPathfindVisibility() const;
	sf::Color GetPathfindColor() const;

	void TogglePathfindingVisibility();

	void SetTarget(Entity* target);
	void SetEnemiesManager(EnemiesManager* manager);
	void SetCollisionsManager(CollisionsManager* manager);
	void SetPathfindPoints(const std::vector<sf::Vector2f>& points);

};

