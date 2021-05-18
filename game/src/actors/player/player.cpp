#include <actors/player.h>
#include <actors/bullet.h>
#include <input.h>
#include <utils/mathUtils.h>
#include <world.h>
#include <tweeners.h>

#include <resources.h>

wok::Player::Player() :
	texture(res::get<sf::Texture>("actors/shrimp")),
	body(*texture, sf::IntRect(0, 0, 13, 14)),
	gun(*texture, sf::IntRect(0, 17, 6, 4)),
	muzzleFlash(*texture, sf::IntRect(7, 15, 7, 8)),
	muzzleFlashOffset({5, 0}),
	gunLine(1.0f)
{
	auto playerSpriteRect = body.getTextureRect();

	sf::Vector2f pivot(
		(float)(playerSpriteRect.width / 2),
		(float)(playerSpriteRect.height)
	);

	auto gunOffset = sf::Vector2f(8, 10);
	auto gunOrigin = sf::Vector2f(1, 1);

	this->gunOffset = gunOffset - pivot;
	body.setOrigin(pivot);
	gun.setOrigin(gunOrigin);
	gunLine.setColor(sf::Color(0xff000022));

	muzzleFlash.setOrigin(0, muzzleFlash.getTextureRect().height / 2.f);
}

void wok::Player::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

void wok::Player::setPosition(sf::Vector2f v)
{
	body.setPosition(v);
}

void wok::Player::update(const GameClock& time)
{
	auto mousePosition = input::mousePositionInWorld;

	body.move(m::normalize(input::movement) * time.delta * 50.f);
	if (mousePosition.x < body.getPosition().x == facingRight)
	{
		facingRight = !facingRight;

		float nextScale = facingRight ? 1.f : -1.f;

		if (flipTween)
			flipTween->kill();

		flipTween = std::make_shared<LerpTweener<float>>(handle,
			[this]() { return body.getScale().x; }, [this](float v) { body.setScale(v, 1); },
			nextScale, 0.1f
		);

		world::addTween(flipTween);
	}

	auto globalGunPos = body.getPosition() + m::scale(gunOffset, body.getScale());
	auto rightDirection = sf::Vector2f(body.getScale().x, 0);

	float angleOfGun = m::angle(rightDirection, mousePosition - globalGunPos);

	gun.setPosition(globalGunPos);
	gun.setRotation(angleOfGun);
	gun.setScale(body.getScale());


	sf::Vector2f gunDirection = m::rotate(sf::Vector2f(body.getScale().x, 0), gun.getRotation());
	m::Ray gunRay(gun.getPosition(), gunDirection);

	if (input::attack.wasPressedThisFrame)
	{
		gunLine.setColor(sf::Color(0xff0000ff));
		auto flash = std::make_shared<LerpTweener<sf::Color>>(handle,
			[this]() { return gunLine.getColor(); }, [this](auto v) { gunLine.setColor(v); },
			sf::Color(0xff000022), 0.3f
			);
		world::addTween(flash);


		muzzleFlash.setPosition(globalGunPos + m::rotate(muzzleFlashOffset, m::angle(gunRay.direction)));
		muzzleFlash.setRotation(gun.getRotation());
		muzzleFlash.setScale(body.getScale());

		renderMuzzleFlash = true;
		muzzleFlash.setColor(sf::Color(0xFFFFFFFF));

		auto muzzleFlashAnimation = std::make_shared<LerpTweener<sf::Color>>(handle,
			[this]() { return muzzleFlash.getColor(); }, [this](auto v) { muzzleFlash.setColor(v); },
			sf::Color(0xFFFFFF00), 0.1f
			);
		muzzleFlashAnimation->setAfterKilled([this]() { renderMuzzleFlash = false; });

		world::addTween(muzzleFlashAnimation);

		world::createNamedActor<Bullet>("Bullet",
			gunRay.origin, gunRay.direction);

		//auto& raycastResult = world::raycast(gunRay);
		//if (raycastResult.hitActor.isValid())
		//{
		//	cs::Print(raycastResult.hitActor.as<Actor>()->name);
		//	raycastResult.hitActor->reactToHit(raycastResult.intersection, 1);
		//}
	}

	gunLine.setStart(gunRay.origin);
	gunLine.setEnd(gunRay.getPoint(1000));
}

void wok::Player::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(body);
	//target.draw(gunLine);
	target.draw(gun);

	if (renderMuzzleFlash)
	{
		target.draw(muzzleFlash);
	}
}
