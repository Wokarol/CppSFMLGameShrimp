#pragma once

#include <physics/intersections.h>
#include <actorHandle.h>

namespace wok::physics
{
	struct RaycastResult
	{
		intersect::Intersection intersection;
		ActorHandle<Actor> hitActor;
	};
}