#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

#include <customShapes/line.h>

namespace wok
{
	class Player : public Actor, public wok::Drawable, public Tickable
	{
		const float shootInterval = 0.5f;
		const float bulletSpread = 1.f;
		const float muzzleFlashTime = 0.05f;

		const float flipTime = 0.1f;

		const float movementSpeed = 50.f;

	public:
		Player();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f v);
		virtual void update(const GameClock& time) override;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

		virtual float getSortingYPos() override { return body.getPosition().y; }

	private:
		std::shared_ptr<sf::Texture> texture;

		sf::Sprite body;
		sf::Sprite gun;
		sf::Sprite muzzleFlash;

		sf::Vector2f gunOffset;
		sf::Vector2f muzzleFlashOffset;
		std::shared_ptr<LerpTweener<float>> flipTween;

		bool facingRight = true;
		bool renderMuzzleFlash = false;
		float shootCooldown = 0;
	};
}