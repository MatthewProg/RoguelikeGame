#include "MathHelper.h"

float MathHelper::GetAngleBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second)
{
    auto rad = atan2f(second.y - first.y, second.x - first.x);
    return RadToDeg(rad);
}

float MathHelper::GetDistanceBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second)
{
    return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

sf::Vector2f MathHelper::GetPointFromAngle(const sf::Vector2f& start, float angle, float radius)
{
    float radAngle = DegToRad(angle);
    return sf::Vector2f(cos(radAngle) * radius + start.x, sin(radAngle) * radius + start.y);
}

sf::Vector2f MathHelper::GetLinesIntersection(const sf::Vector2f& startPos1, const sf::Vector2f& endPos1, const sf::Vector2f& startPos2, const sf::Vector2f& endPos2)
{
    sf::Vector2f s1 = endPos1 - startPos1;
    sf::Vector2f s2 = endPos2 - startPos2;

    float s, t;
    s = (-s1.y * (startPos1.x - startPos2.x) + s1.x * (startPos1.y - startPos2.y)) / (-s2.x * s1.y + s1.x * s2.y);
    t = (s2.x * (startPos1.y - startPos2.y) - s2.y * (startPos1.x - startPos2.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) // Collision detected
    {
        sf::Vector2f output;
        output.x = startPos1.x + (t * s1.x);
        output.y = startPos1.y + (t * s1.y);
        return output;
    }

    return endPos1; // No collision
}

float MathHelper::RadToDeg(float rad)
{
    return (float)((double)rad * 180.0 / PI);
}

float MathHelper::DegToRad(float deg)
{
    return (float)(deg * PI / 180.0);
}