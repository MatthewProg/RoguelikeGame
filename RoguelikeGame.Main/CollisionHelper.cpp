#include "CollisionHelper.h"

bool CollisionHelper::CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second)
{
    return first.intersects(second);
}

bool CollisionHelper::CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<uint8_t> *tiles, const uint8_t blockId)
{
    unsigned int y1 = (uint8_t)(obj.top / tiles->tileHeight);
    unsigned int y2 = (uint8_t)(((double)obj.top + obj.height - .01) / tiles->tileHeight);
    unsigned int x1 = (uint8_t)(obj.left / tiles->tileWidth);
    unsigned int x2 = (uint8_t)(((double)obj.left + obj.width - .01) / tiles->tileWidth);

    if (tiles->data[y1 * tiles->width + x1] == blockId) return true;
    if (tiles->data[y1 * tiles->width + x2] == blockId) return true;
    if (tiles->data[y2 * tiles->width + x1] == blockId) return true;
    if (tiles->data[y2 * tiles->width + x2] == blockId) return true;

    return false;
}

sf::Vector2f CollisionHelper::GetLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<uint8_t>* tiles, const uint8_t blockId)
{
    if (CheckTileCollision(endPos, tiles, blockId))
    {
        //Start pos near by tiles
        unsigned int yStart1 = (uint8_t)(startPos.top / tiles->tileHeight);
        unsigned int yStart2 = (uint8_t)(((double)startPos.top + startPos.height - .01) / tiles->tileHeight);
        unsigned int xStart1 = (uint8_t)(startPos.left / tiles->tileWidth);
        unsigned int xStart2 = (uint8_t)(((double)startPos.left + startPos.width - .01 ) / tiles->tileWidth);

        sf::Vector2f moveVector(endPos.left - startPos.left, endPos.top - startPos.top);

        float outY = startPos.top;
        float outX = startPos.left;

        if (moveVector.y < 0) //Trying to move up
        {
            if (yStart1 > 0) //Not edge
            {
                if (tiles->data[(yStart1 - 1) * tiles->width + xStart1] == blockId || tiles->data[(yStart1 - 1) * tiles->width + xStart2] == blockId) //Collision on top
                {
                    auto tileLowerY = ((yStart1 - 1) * tiles->tileHeight) + tiles->tileHeight;
                    auto diff = startPos.top - tileLowerY;
                    if ((moveVector.y * -1) < diff)
                        outY += moveVector.y;
                    else
                        outY -= diff;
                }
                else
                    outY += moveVector.y;
            }
        }
        else if (moveVector.y > 0) //Trying to move down
        {
            if (yStart2 < tiles->height - 1) //Not edge
            {
                if (tiles->data[(yStart2 + 1) * tiles->width + xStart1] == blockId || tiles->data[(yStart2 + 1) * tiles->width + xStart2] == blockId) //Collision on bottom
                {
                    auto tileUpperY = ((yStart2 + 1) * tiles->tileHeight);
                    auto diff = tileUpperY - (startPos.top + startPos.height);
                    if (moveVector.y < diff)
                        outY += moveVector.y;
                    else
                        outY += diff;
                }
                else
                    outY += moveVector.y;
            }
        }

        if (moveVector.x < 0) //Trying to move left
        {
            if (xStart1 > 0) //Not edge
            {
                if (tiles->data[yStart1 * tiles->width + (xStart1 - 1)] == blockId || tiles->data[yStart2 * tiles->width + (xStart1 - 1)] == blockId) //Collision on left
                {
                    auto tileRightX = ((xStart1-1) * tiles->tileWidth) + tiles->tileWidth;
                    auto diff = startPos.left - tileRightX;
                    if ((moveVector.x * -1) < diff)
                        outX += moveVector.x;
                    else
                        outX -= diff;
                }
                else
                    outX += moveVector.x;
            }
        }
        else if (moveVector.x > 0) //Trying to move right
        {
            if (xStart2 < tiles->width - 1) //Not edge
            {
                if (tiles->data[yStart1 * tiles->width + (xStart2 + 1)] == blockId || tiles->data[yStart2 * tiles->width + (xStart2 + 1)] == blockId) //Collision on right
                {
                    auto tileLeftX = ((xStart2 + 1) * tiles->tileWidth);
                    auto diff = tileLeftX - (startPos.left + startPos.width);
                    if (moveVector.x < diff)
                        outX += moveVector.x;
                    else
                        outX += diff;
                }
                else
                    outX += moveVector.x;
            }
        }
        return sf::Vector2f(outX, outY);
    }
    else
        return sf::Vector2f(endPos.left, endPos.top);
}
