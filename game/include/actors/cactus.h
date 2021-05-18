#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <tweeners.h>

namespace wok
{
	class Cactus : public sf::Sprite,
		public Actor, public wok::Drawable, public Hittable
	{
		std::shared_ptr<sf::Texture> texture;
		std::shared_ptr<SineTweener<float>> animation;
		float sizeScale;

	public:
		Cactus(std::shared_ptr<sf::Texture> texture, sf::IntRect rect, float sizeScale);

		virtual void start() override;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;;
		virtual float getSortingYPos() override { return getPosition().y; }
		virtual intersect::Intersection getClosestHit(const m::Ray& ray) override;
		virtual void reactToHit(const intersect::Intersection& intersection) override;
	};
}
