#pragma once

#include <actor.h>

namespace wok::physics
{
    struct RaycastResult
    {
        Intersection intersection;
        ActorHandle<Collideable> collideable;
    };
}
