#include "ViewHelper.h"

sf::Vector2f ViewHelper::GetRectCenter(const sf::FloatRect& rect)
{
    float x = rect.left + rect.width / 2;
    float y = rect.top + rect.height / 2;
    return sf::Vector2f(x, y);
}

sf::FloatRect ViewHelper::GetScaled(const sf::FloatRect& scale, const sf::FloatRect& element, const sf::FloatRect& relativeTo)
{
    float xScale = std::max(0.f, std::min(1.f, scale.left));
    float yScale = std::max(0.f, std::min(1.f, scale.top));
    float widthScale = std::max(0.f, std::min(1.f, scale.width));
    float heightScale = std::max(0.f, std::min(1.f, scale.height));

    float xR = relativeTo.left + (relativeTo.width * xScale);
    float yR = relativeTo.top + (relativeTo.height * yScale);
    float w = element.width * widthScale;
    float h = element.height * heightScale;
    float x = xR - (w / 2.f);
    float y = yR - (h / 2.f);

    return sf::FloatRect(x, y, w, h);
}
