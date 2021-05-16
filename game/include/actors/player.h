#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

class Player : public Actor, public Drawable
{
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite body;
	sf::Sprite gun;
	sf::Vector2f gunOffset;
	std::shared_ptr<LerpTweener<float>> flipTween;

	bool facingRight = true;

public:
	Player(std::shared_ptr<sf::Texture> texture, 
		sf::IntRect playerSpriteRect, sf::IntRect gunSpriteRect, 
		sf::Vector2f gunOffset, sf::Vector2f gunOrigin):
		texture(texture),
		body(*texture, playerSpriteRect),
		gun(*texture, gunSpriteRect)
	{
		sf::Vector2f pivot(
			(float)(playerSpriteRect.width / 2), 
			(float)(playerSpriteRect.height)
		);
		this->gunOffset = gunOffset - pivot;
		body.setOrigin(pivot);
		gun.setOrigin(gunOrigin);
	}

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f v);
	virtual void update(const GameClock& time) override;
	virtual void draw(sf::RenderTarget& target) override;
};