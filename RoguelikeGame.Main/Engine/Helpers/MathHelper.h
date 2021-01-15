#pragma once

#include <cmath>

#include "SFML/System/Vector2.hpp"

#define PI 3.14159265359

class MathHelper
{
public:
	static float GetAngleBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second);
	static float GetDistanceBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second);
	static sf::Vector2f GetPointFromAngle(const sf::Vector2f& start, float angle, float radius);
	static sf::Vector2f GetLinesIntersection(const sf::Vector2f& startPos1, const sf::Vector2f& endPos1, const sf::Vector2f& startPos2, const sf::Vector2f& endPos2);
	static float RadToDeg(float rad);
	static float DegToRad(float deg);
};

