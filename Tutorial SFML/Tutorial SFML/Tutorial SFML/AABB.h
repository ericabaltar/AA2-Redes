#pragma once
#include "Vector2.h"
#include <cmath>
#include <algorithm>
#include <iostream>

struct CollisionInfo
{
    bool collided = false;
    Vector2 normal = Vector2(0.f, 0.f);
    float penetration = 0.f;
};

class AABB
{
protected:
    Vector2 topLeft;
    Vector2 size;

public:
    AABB(Vector2 _topLeft, Vector2 _size)
        : topLeft(_topLeft), size(_size) {
    }

    inline Vector2 GetTopLeft() { return topLeft; };
    inline Vector2 GetSize() { return size; };
    inline void SetTopLeft(Vector2 newTopLeft) { topLeft = newTopLeft; };
    inline void SetSize(Vector2 newSize) { size = newSize; };

    bool CheckOverlappingPoint(Vector2 point)
    {
        bool insideX = point.x >= topLeft.x && point.x <= topLeft.x + size.x;
        bool insideY = point.y >= topLeft.y && point.y <= topLeft.y + size.y;

        return insideX && insideY;
    }

    bool CheckOverlappingAABB(const AABB* other)
    {
        bool intersectionX = false;
        intersectionX |= topLeft.x >= other->topLeft.x && topLeft.x <= other->topLeft.x + other->size.x;
        intersectionX |= other->topLeft.x >= topLeft.x && other->topLeft.x <= topLeft.x + size.x;

        bool intersectionY = false;
        intersectionY |= topLeft.y >= other->topLeft.y && topLeft.y <= other->topLeft.y + other->size.y;
        intersectionY |= other->topLeft.y >= topLeft.y && other->topLeft.y <= topLeft.y + size.y;

        return intersectionX && intersectionY;
    }

    CollisionInfo GetCollisionInfo(const AABB* other)
    {
        float axMin = topLeft.x;
        float axMax = topLeft.x + size.x;
        float ayMin = topLeft.y;
        float ayMax = topLeft.y + size.y;

        float bxMin = other->topLeft.x;
        float bxMax = other->topLeft.x + other->size.x;
        float byMin = other->topLeft.y;
        float byMax = other->topLeft.y + other->size.y;

        float overlapX = std::min(axMax, bxMax) - std::max(axMin, bxMin);
        float overlapY = std::min(ayMax, byMax) - std::max(ayMin, byMin);

        CollisionInfo info;

        if (overlapX <= 0 || overlapY <= 0)
            return info;

        info.collided = true;

        if (overlapX < overlapY)
        {
            info.penetration = overlapX;

            if (axMin < bxMin)
                info.normal = Vector2(-1.f, 0.f);
            else
                info.normal = Vector2(1.f, 0.f);
        }
        else
        {
            info.penetration = overlapY;

            if (ayMin < byMin)
                info.normal = Vector2(0.f, -1.f);
            else
                info.normal = Vector2(0.f, 1.f);
        }

        return info;
    }
};