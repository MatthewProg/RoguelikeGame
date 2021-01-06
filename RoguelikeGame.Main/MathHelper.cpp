#include "MathHelper.h"

float MathHelper::GetAngleBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second)
{
    auto rad = atan2((double)second.y - (double)first.y, (double)second.x - (double)first.x);
    return RadToDeg((float)rad);
}

float MathHelper::GetLenghtBetweenPoints(const sf::Vector2f& first, const sf::Vector2f& second)
{
    return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

sf::Vector2f MathHelper::GetPointFromAngle(const sf::Vector2f& start, float angle, float radius)
{
    float radAngle = DegToRad(angle);
    return sf::Vector2f((float)cos(radAngle) * radius + start.x, (float)sin(radAngle) * radius + start.y);
}

float MathHelper::RadToDeg(float rad)
{
    return (float)((double)rad * 180.0 / PI);
}

float MathHelper::DegToRad(float deg)
{
    return (float)(deg * PI / 180);
}
