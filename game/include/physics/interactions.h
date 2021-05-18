#pragma once

#include <physics/intersections.h>
#include <actor.h>

namespace wok::physics
{
	struct RaycastResult
	{
		intersect::Intersection intersection;
		ActorHandle<Hittable> hitActor;
	};
}