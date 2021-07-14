#include "CollisionHelper.h"

bool CollisionHelper::CheckSimpleCollision(const sf::FloatRect& first, const sf::FloatRect& second)
{
    return first.intersects(second);
}

bool CollisionHelper::CheckCircleCollision(const sf::Vector2f& point, const sf::Vector2f& center, float radius, float arc, float angle)
{
    auto pointAngle = MathHelper::GetAngleBetweenPoints(center, point);
    auto pointRadius = MathHelper::GetDistanceBetweenPoints(center, point);
    auto normalPointAngle = (pointAngle < 0) ? 360 + pointAngle : pointAngle;
    auto normalCircleAngleL = ((angle < 0) ? 360 + angle : angle) - (arc / 2);
    auto normalCircleAngleR = ((angle < 0) ? 360 + angle : angle) + (arc / 2);

    if (normalCircleAngleL < normalCircleAngleR) //Not crossing 0deg
        return (normalPointAngle >= normalCircleAngleL && normalPointAngle <= normalCircleAngleR && pointRadius <= radius);
    else //Crossing 0deg
        return((normalPointAngle >= normalCircleAngleL && normalPointAngle <= 360 && pointRadius <= radius) ||
                normalPointAngle <= normalCircleAngleR && normalPointAngle >= 0 && pointRadius <= radius);
}

bool CollisionHelper::CheckCircleCollision(const sf::Vector2f& point, const sf::Vector2f& center, float radius)
{
    auto diff = point - center;
    return (diff.x * diff.x + diff.y * diff.y <= radius * radius);
}

bool CollisionHelper::CheckCirclesIntersect(const sf::Vector2f& center1, float radius1, const sf::Vector2f& center2, float radius2)
{
    return (MathHelper::GetDistanceBetweenPoints(center1, center2) <= radius1 + radius2);
}

bool CollisionHelper::CheckTileCollision(const sf::FloatRect& obj, const MapLayerModel<bool> *tiles)
{
    auto pos = GetPosOnTiles(obj, tiles);
    if (pos.y < 0 || pos.y >(int)tiles->height - 1 ||
        pos.w < 0 || pos.w >(int)tiles->height - 1 ||
        pos.x < 0 || pos.x >(int)tiles->width - 1 ||
        pos.z < 0 || pos.z >(int)tiles->width - 1)
        return false;

    if (tiles->data[pos.y * tiles->width + pos.x]) return true;
    if (tiles->data[pos.y * tiles->width + pos.z]) return true;
    if (tiles->data[pos.w * tiles->width + pos.x]) return true;
    if (tiles->data[pos.w * tiles->width + pos.z]) return true;

    return false;
}

bool CollisionHelper::CheckTileCollision(const sf::Vector2f& center, float radius, const MapLayerModel<bool>* tiles)
{
    auto tl = GetPosOnTiles(sf::Vector2f(center.x - radius, center.y - radius), tiles);
    auto dr = GetPosOnTiles(sf::Vector2f(center.x + radius, center.y + radius), tiles);

    for (int y = tl.y; y <= dr.y; y++)
        for (int x = tl.x; x <= dr.x; x++)
        {
            if (x < 0 || y < 0 || x > (int)tiles->width - 1 || y > (int)tiles->height - 1) continue;
            if (tiles->data[y * tiles->width + x] == false) continue;

            auto tileCenter = sf::Vector2f(x * tiles->tileWidth + (tiles->tileWidth / 2) - tiles->offsetX,
                                           y * tiles->tileHeight + (tiles->tileHeight / 2) - tiles->offsetY);
            sf::Vector2f circleDistance;
            circleDistance.x = abs(center.x - tileCenter.x);
            circleDistance.y = abs(center.y - tileCenter.y);

            if (circleDistance.x > (tiles->tileWidth / 2.f + radius)) { continue; }
            if (circleDistance.y > (tiles->tileHeight / 2.f + radius)) { continue; }

            if (circleDistance.x <= (tiles->tileWidth / 2.f)) { return true; }
            if (circleDistance.y <= (tiles->tileHeight / 2.f)) { return true; }

            auto cX = circleDistance.x - (tiles->tileWidth / 2.f);
            auto cY = circleDistance.y - (tiles->tileHeight / 2.f);
            if (cX * cX + cY * cY <= radius * radius) return true;
        }
    return false;
}

bool CollisionHelper::CheckRectContains(const sf::IntRect& outer, const sf::IntRect& inner)
{
    return (inner.left >= outer.left && inner.left + inner.width <= outer.left + outer.width &&
        inner.top >= outer.top && inner.top + inner.height <= outer.top + outer.height);
}

bool CollisionHelper::CheckRectContains(const sf::FloatRect& outer, const sf::FloatRect& inner)
{
    return (inner.left >= outer.left && inner.left + inner.width <= outer.left + outer.width &&
        inner.top >= outer.top && inner.top + inner.height <= outer.top + outer.height);
}

sf::Vector2f CollisionHelper::GetTileLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos, const MapLayerModel<bool>* tiles)
{
    if (CheckTileCollision(endPos, tiles))
    {
        //Start pos near by tiles
        auto pos = GetPosOnTiles(startPos, tiles);

        sf::Vector2f moveVector(endPos.left - startPos.left, endPos.top - startPos.top);

        float outY = startPos.top;
        float outX = startPos.left;

        if (moveVector.y < 0) //Trying to move up
        {
            if (pos.y > 0) //Not edge
            {
                if (tiles->data[(pos.y - 1) * tiles->width + pos.x] || tiles->data[(pos.y - 1) * tiles->width + pos.z]) //Collision on top
                {
                    auto tileLowerY = ((pos.y - 1) * tiles->tileHeight) + tiles->tileHeight + tiles->offsetY;
                    auto diff = startPos.top - tileLowerY;
                    if ((moveVector.y * -1) < diff)
                        outY += moveVector.y;
                    else
                        outY -= diff;
                }
                else
                    outY += moveVector.y;
            }
            else
                outY += moveVector.y;
        }
        else if (moveVector.y > 0) //Trying to move down
        {
            if (pos.w < (int)tiles->height - 1) //Not edge
            {
                if (tiles->data[(pos.w + 1) * tiles->width + pos.x] || tiles->data[(pos.w + 1) * tiles->width + pos.z]) //Collision on bottom
                {
                    auto tileUpperY = ((pos.w + 1) * tiles->tileHeight) + tiles->offsetY;
                    auto diff = tileUpperY - (startPos.top + startPos.height);
                    if (moveVector.y < diff)
                        outY += moveVector.y;
                    else
                        outY += diff;
                }
                else
                    outY += moveVector.y;
            }
            else
                outY += moveVector.y;
        }

        //Recalc pos if diaognal move
        if(moveVector.x != 0 && moveVector.y != 0)
            pos = GetPosOnTiles(sf::FloatRect(outX, outY, startPos.width, startPos.height), tiles);

        if (moveVector.x < 0) //Trying to move left
        {
            if (pos.x > 0) //Not edge
            {
                if (tiles->data[pos.y * tiles->width + (pos.x - 1)] || tiles->data[pos.w * tiles->width + (pos.x - 1)]) //Collision on left
                {
                    auto tileRightX = ((pos.x-1) * tiles->tileWidth) + tiles->tileWidth + tiles->offsetX;
                    auto diff = startPos.left - tileRightX;
                    if ((moveVector.x * -1) < diff)
                        outX += moveVector.x;
                    else
                        outX -= diff;
                }
                else
                    outX += moveVector.x;
            }
            else
                outX += moveVector.x;
        }
        else if (moveVector.x > 0) //Trying to move right
        {
            if (pos.z < (int)tiles->width - 1) //Not edge
            {
                if (tiles->data[pos.y * tiles->width + (pos.z + 1)] || tiles->data[pos.w * tiles->width + (pos.z + 1)]) //Collision on right
                {
                    auto tileLeftX = ((pos.z + 1) * tiles->tileWidth) + tiles->offsetX;
                    auto diff = tileLeftX - (startPos.left + startPos.width);
                    if (moveVector.x < diff)
                        outX += moveVector.x;
                    else
                        outX += diff;
                }
                else
                    outX += moveVector.x;
            }
            else
                outX += moveVector.x;
        }
        return sf::Vector2f(outX, outY);
    }
    else
        return sf::Vector2f(endPos.left, endPos.top);
}

sf::Vector2f CollisionHelper::GetTileLimitPosition(const sf::Vector2f& startPos, const sf::Vector2f& endPos, float radius, const MapLayerModel<bool>* tiles)
{
    //Circle tile collision
    sf::Vector2f output = endPos;

    float precision = 0.05F;
    auto tl = GetPosOnTiles(sf::Vector2f(endPos.x - radius, endPos.y - radius), tiles);
    auto dr = GetPosOnTiles(sf::Vector2f(endPos.x + radius, endPos.y + radius), tiles);

    for (int y = tl.y; y <= dr.y; y++)
        for (int x = tl.x; x <= dr.x; x++)
        {
            if (x < 0 || y < 0 || x > (int)tiles->width - 1 || y > (int)tiles->height - 1) continue;
            if (tiles->data[y * tiles->width + x] == false) continue;

            auto tileLeft = x * tiles->tileWidth - tiles->offsetX - precision;
            auto tileRight = x * tiles->tileWidth + tiles->tileWidth - tiles->offsetX + precision;
            auto tileTop = y * tiles->tileHeight - tiles->offsetY - precision;
            auto tileDown = y * tiles->tileHeight + tiles->tileHeight - tiles->offsetY + precision;


            auto closestX = std::max(tileLeft, std::min(tileRight, output.x));
            auto closestY = std::max(tileTop, std::min(tileDown, output.y));
            auto closest = sf::Vector2f(closestX, closestY);

            sf::FloatRect tile(tileLeft, tileTop, tileRight - tileLeft, tileDown - tileTop); //If endpos center is inside block
            if (tile.contains(endPos))
                output = closest;

            auto moveVector = output - startPos;
            auto len = MathHelper::GetDistanceBetweenPoints(output, closest);
            auto diff = radius - len;
            if (diff > 0)
            {
                auto backVector = output - closest;

                //Nomalize
                if (len > 0)
                {
                    backVector.x = backVector.x / len;
                    backVector.y = backVector.y / len;
                }

                output.x = startPos.x + moveVector.x + (backVector.x * diff);
                output.y = startPos.y + moveVector.y + (backVector.y * diff);
            }
        }

    return output;
}

sf::Vector2f CollisionHelper::GetRectLimitPosition(const sf::FloatRect& startPos, const sf::FloatRect& endPos, const sf::FloatRect& block)
{
    if (CheckSimpleCollision(endPos, block))
    {
        sf::Vector2f moveVector(endPos.left - startPos.left, endPos.top - startPos.top);

        float outY = startPos.top;
        float outX = startPos.left;

        sf::FloatRect verticalMove(startPos.left, endPos.top, startPos.width, startPos.height);
        if (moveVector.y < 0) //Trying to move up
        {
            if (CheckSimpleCollision(verticalMove, block)) //Collision on top
            {
                auto blockLowerY = block.top + block.height;
                auto diff = startPos.top - blockLowerY;
                outY -= diff;
            }
            else
                outY += moveVector.y;
        }
        else if (moveVector.y > 0) //Trying to move down
        {
            if (CheckSimpleCollision(verticalMove, block))
            {
                auto blockUpperY = block.top;
                auto diff = blockUpperY - (startPos.top + startPos.height);
                outY += diff;
            }
            else
                outY += moveVector.y;
        }

        //Make moves
        sf::FloatRect horizontalMove(endPos.left, outY, startPos.width, startPos.height);

        if (moveVector.x < 0) //Trying to move left
        {
            if (CheckSimpleCollision(horizontalMove,block))
            {
                auto blockRightX = block.left + block.width;
                auto diff = startPos.left - blockRightX;
                outX -= diff;
            }
            else
                outX += moveVector.x;
        }
        else if (moveVector.x > 0) //Trying to move right
        {
            if (CheckSimpleCollision(horizontalMove, block))
            {
                auto blockLeftX = block.left;
                auto diff = blockLeftX - (startPos.left + startPos.width);
                outX += diff;
            }
            else
                outX += moveVector.x;
        }
        
        return sf::Vector2f(outX, outY);
    }
    else
        return sf::Vector2f(endPos.left, endPos.top);
}

sf::Glsl::Ivec4 CollisionHelper::GetPosOnTiles(const sf::FloatRect& pos, const MapLayerModel<bool>* tiles)
{
    int y1 = (int)((pos.top - tiles->offsetY) / tiles->tileHeight);
    int y2 = (int)(((double)pos.top + pos.height - tiles->offsetY - .01) / tiles->tileHeight);
    int x1 = (int)((pos.left - tiles->offsetX) / tiles->tileWidth);
    int x2 = (int)(((double)pos.left + pos.width - tiles->offsetX - .01) / tiles->tileWidth);
    return sf::Glsl::Ivec4(x1, y1, x2, y2);
}

sf::Vector2i CollisionHelper::GetPosOnTiles(const sf::Vector2f& pos, const MapLayerModel<bool>* tiles)
{
    int x = (int)((pos.x - tiles->offsetX) / tiles->tileWidth);
    int y = (int)((pos.y - tiles->offsetY) / tiles->tileHeight);
    return sf::Vector2i(x, y);
}

std::vector<sf::Vector2f> CollisionHelper::GetRectPoints(const sf::FloatRect& rect)
{
    std::vector<sf::Vector2f> output(4);

    output[0] = sf::Vector2f(rect.left, rect.top);
    output[1] = sf::Vector2f(rect.left + rect.width, rect.top);
    output[2] = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    output[3] = sf::Vector2f(rect.left, rect.top + rect.height);

    return output;
}
