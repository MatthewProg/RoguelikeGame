#include "CollisionHelper.h"

bool CollisionHelper::CheckSimpleCollision(const sf::FloatRect first, const sf::FloatRect second)
{
    return first.intersects(second);
}

bool CollisionHelper::CheckTileCollision(const sf::FloatRect obj, const MapLayerModel<bool> *tiles)
{
    int y1 = (int)((obj.top - tiles->offsetY) / tiles->tileHeight);
    int y2 = (int)(((double)obj.top + obj.height - tiles->offsetY - .01) / tiles->tileHeight);
    int x1 = (int)((obj.left - tiles->offsetX) / tiles->tileWidth);
    int x2 = (int)(((double)obj.left + obj.width - tiles->offsetX - .01) / tiles->tileWidth);

    if (y1 < 0 || y1 >(int)tiles->height - 1 ||
        y2 < 0 || y2 >(int)tiles->height - 1 ||
        x1 < 0 || x1 >(int)tiles->width - 1 ||
        x2 < 0 || x2 >(int)tiles->width - 1)
        return false;

    if (tiles->data[y1 * tiles->width + x1]) return true;
    if (tiles->data[y1 * tiles->width + x2]) return true;
    if (tiles->data[y2 * tiles->width + x1]) return true;
    if (tiles->data[y2 * tiles->width + x2]) return true;

    return false;
}

sf::Vector2f CollisionHelper::GetTileLimitPosition(const sf::FloatRect startPos, const sf::FloatRect endPos, const MapLayerModel<bool>* tiles)
{
    if (CheckTileCollision(endPos, tiles))
    {
        //Start pos near by tiles
        int yStart1 = (int)((startPos.top - tiles->offsetY) / tiles->tileHeight);
        int yStart2 = (int)(((double)startPos.top + startPos.height - tiles->offsetY - .01) / tiles->tileHeight);
        int xStart1 = (int)((startPos.left - tiles->offsetX) / tiles->tileWidth);
        int xStart2 = (int)(((double)startPos.left + startPos.width - tiles->offsetX - .01) / tiles->tileWidth);

        sf::Vector2f moveVector(endPos.left - startPos.left, endPos.top - startPos.top);

        float outY = startPos.top;
        float outX = startPos.left;

        if (moveVector.y < 0) //Trying to move up
        {
            if (yStart1 > 0) //Not edge
            {
                if (tiles->data[(yStart1 - 1) * tiles->width + xStart1] || tiles->data[(yStart1 - 1) * tiles->width + xStart2]) //Collision on top
                {
                    auto tileLowerY = ((yStart1 - 1) * tiles->tileHeight) + tiles->tileHeight + tiles->offsetY;
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
            if (yStart2 < (int)tiles->height - 1) //Not edge
            {
                if (tiles->data[(yStart2 + 1) * tiles->width + xStart1] || tiles->data[(yStart2 + 1) * tiles->width + xStart2]) //Collision on bottom
                {
                    auto tileUpperY = ((yStart2 + 1) * tiles->tileHeight) + tiles->offsetY;
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

        if (moveVector.x < 0) //Trying to move left
        {
            if (xStart1 > 0) //Not edge
            {
                if (tiles->data[yStart1 * tiles->width + (xStart1 - 1)] || tiles->data[yStart2 * tiles->width + (xStart1 - 1)]) //Collision on left
                {
                    auto tileRightX = ((xStart1-1) * tiles->tileWidth) + tiles->tileWidth + tiles->offsetX;
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
            if (xStart2 < (int)tiles->width - 1) //Not edge
            {
                if (tiles->data[yStart1 * tiles->width + (xStart2 + 1)] || tiles->data[yStart2 * tiles->width + (xStart2 + 1)]) //Collision on right
                {
                    auto tileLeftX = ((xStart2 + 1) * tiles->tileWidth) + tiles->offsetX;
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
