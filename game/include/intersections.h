#pragma once

#include <mathUtils.h>
#include <SFML/Graphics.hpp>

#include <console.h>

namespace inter
{
	struct Intersection
	{
		bool hit;
		float dist;
		sf::Vector2f normal;

		Intersection() : hit(false), dist(0), normal(sf::Vector2f(0.f, 0.f)) {}
		Intersection(float d, sf::Vector2f n) : hit(true), dist(d), normal(n) {}
	};

	/// <param name="AABB"> Rectangle has to be AABB </param>
	inline Intersection rayAABB(sf::Vector2f origin, sf::Vector2f dir, sf::RectangleShape aabb)
	{
		origin += aabb.getOrigin();
		dir = m::normalize(dir);
		auto size = aabb.getSize();

		if (dir.x > 0 && origin.x < 0)
		{
			// Ray from left onto left wall
			float diff = -origin.x;
			float distanceToWall = diff / dir.x;
			float yOnWall = dir.y * distanceToWall + origin.y;

			if (yOnWall >= 0 && yOnWall <= size.y)
			{
				return Intersection(distanceToWall, sf::Vector2f(-1.f, 0.f));
			}
		}

		if (dir.x < 0 && origin.x > size.x)
		{
			// Ray from right onto right wall
			float diff = origin.x - size.x;
			float distanceToWall = -diff / dir.x;
			float yOnWall = dir.y * distanceToWall + origin.y;

			if (yOnWall >= 0 && yOnWall <= size.y)
			{
				return Intersection(distanceToWall, sf::Vector2f(1.f, 0.f));
			}
		}

		if (dir.y > 0 && origin.y < 0)
		{
			// Ray from up onto upper wall
			float diff = -origin.y;
			float distanceToWall = diff / dir.y;
			float xOnWall = dir.x * distanceToWall + origin.x;

			if (xOnWall >= 0 && xOnWall <= size.x)
			{
				return Intersection(distanceToWall, sf::Vector2f(0.f, -1.f));
			}
		}

		if (dir.y < 0 && origin.y > size.y)
		{
			// Ray from right onto right wall
			float diff = origin.y - size.y;
			float distanceToWall = -diff / dir.y;
			float xOnWall = dir.x * distanceToWall + origin.x;

			if (xOnWall >= 0 && xOnWall <= size.x)
			{
				return Intersection(distanceToWall, sf::Vector2f(0.f, 1.f));
			}
		}

		return {};
	}
}