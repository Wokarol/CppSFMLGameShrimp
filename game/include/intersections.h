#pragma once

#include <mathUtils.h>
#include <SFML/Graphics.hpp>

namespace intersect
{
	struct Intersection
	{
		bool hit;
		float distance;
		sf::Vector2f normal;

		Intersection() : hit(false), distance(0), normal(sf::Vector2f(0.f, 0.f)) {}
		Intersection(float d, sf::Vector2f n) : hit(true), distance(d), normal(n) {}
	};

	inline Intersection rayWithCircle(m::Ray ray, const sf::CircleShape& circle)
	{

		// We offset it so the centre of the circle is in the middle
		sf::Vector2f optimalPivot = sf::Vector2f(1.f, 1.f) * circle.getRadius();
		sf::Vector2f pivotOffset = circle.getOrigin() - optimalPivot;

		ray.move(-circle.getPosition() + pivotOffset);
		ray.direction = m::normalize(ray.direction);

		// Algorithm is based on this article:
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
		sf::Vector2f L = -ray.origin;
		float r = circle.getRadius();

		if (m::sqrLength(L) < (r * r))
		{
			// We are inside the circle
			return {};
		}

		float tca = m::dot(L, ray.direction);
		if (tca < 0)
		{
			// We point away from the circle
			return {};
		}

		float d = std::sqrt(m::dot(L, L) - (tca * tca));
		if (d > r)
		{
			// We do not hit the circle
			return {};
		}

		float thc = std::sqrt(r * r - d * d);
		float distToHit = tca - thc;

		sf::Vector2f hitPoint = ray.getPoint(distToHit);
		sf::Vector2f normal = m::normalize(hitPoint);

		return Intersection(distToHit, normal);
	}

	/// <param name="AABB"> Rectangle has to be AABB </param>
	inline Intersection rayWithAABB(m::Ray ray, const sf::RectangleShape& aabb)
	{
		// We offset it so the aabb's top left corner is at [0, 0]
		ray.move(aabb.getOrigin());
		ray.direction = m::normalize(ray.direction);
		auto size = aabb.getSize();

		if (ray.direction.x > 0 && ray.origin.x < 0)
		{
			// Ray from left onto left wall
			float distanceToWallOnXAxis = -ray.origin.x;
			float distanceToWall = distanceToWallOnXAxis / ray.direction.x;
			float yOnWall = ray.getPoint(distanceToWall).y;

			if (yOnWall >= 0 && yOnWall <= size.y)
			{
				return Intersection(distanceToWall, sf::Vector2f(-1.f, 0.f));
			}
		}

		if (ray.direction.x < 0 && ray.origin.x > size.x)
		{
			// Ray from right onto right wall
			float distanceToWallOnXAxis = ray.origin.x - size.x;
			float distanceToWall = -distanceToWallOnXAxis / ray.direction.x;
			float yOnWall = ray.getPoint(distanceToWall).y;

			if (yOnWall >= 0 && yOnWall <= size.y)
			{
				return Intersection(distanceToWall, sf::Vector2f(1.f, 0.f));
			}
		}

		if (ray.direction.y > 0 && ray.origin.y < 0)
		{
			// Ray from up onto upper wall
			float distanceOnWallOnYAxis = -ray.origin.y;
			float distanceToWall = distanceOnWallOnYAxis / ray.direction.y;
			float xOnWall = ray.getPoint(distanceToWall).x;

			if (xOnWall >= 0 && xOnWall <= size.x)
			{
				return Intersection(distanceToWall, sf::Vector2f(0.f, -1.f));
			}
		}

		if (ray.direction.y < 0 && ray.origin.y > size.y)
		{
			// Ray from down onto bottom wall
			float distanceOnWallOnYAxis = ray.origin.y - size.y;
			float distanceToWall = -distanceOnWallOnYAxis / ray.direction.y;
			float xOnWall = ray.getPoint(distanceToWall).x;

			if (xOnWall >= 0 && xOnWall <= size.x)
			{
				return Intersection(distanceToWall, sf::Vector2f(0.f, 1.f));
			}
		}

		return {};
	}

	inline Intersection rayWithOBB(m::Ray ray, const sf::RectangleShape& obb)
	{
		// We convert this scenario into Ray and AABB to simplify calculations
		ray.move(-obb.getPosition());

		auto angle = -obb.getRotation();
		ray.rotateAround(sf::Vector2f(0, 0), angle);

		auto res = rayWithAABB(ray, obb);
		res.normal = m::rotate(res.normal, -angle);

		return res;
	}

	inline Intersection rayWithAny(m::Ray ray, const sf::Shape& s)
	{
		if (auto circle = dynamic_cast<const sf::CircleShape*>(&s))
		{
			return rayWithCircle(ray, *circle);
		}

		if (auto obb = dynamic_cast<const sf::RectangleShape*>(&s))
		{
			return rayWithOBB(ray, *obb);
		}

		// Our shape is unsupported
		return {};
	}

	template <typename FwdIt>
	inline Intersection raycastAllShapes(m::Ray ray, FwdIt start, FwdIt end)
	{
		Intersection closestIntersection;
		float smallestDistance = std::numeric_limits<float>::infinity();

		for (auto it = start; it < end; it++)
		{
			auto shape = *it;
			auto raycastResult = rayWithAny(ray, *shape);

			if (!raycastResult.hit)
				continue;

			if (raycastResult.distance > smallestDistance)
				continue;

			closestIntersection = raycastResult;
			smallestDistance = raycastResult.distance;
		}

		return closestIntersection;
	}
}