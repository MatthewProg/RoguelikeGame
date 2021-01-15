#include "ViewHelper.h"

sf::Vector2f ViewHelper::GetRectCenter(const sf::FloatRect& rect)
{
    float x = rect.left + rect.width / 2;
    float y = rect.top + rect.height / 2;
    return sf::Vector2f(x, y);
}
