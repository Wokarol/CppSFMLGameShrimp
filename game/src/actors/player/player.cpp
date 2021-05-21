#include <actors/player.h>
#include <actors/bullet.h>
#include <input.h>
#include <utils/mathUtils.h>
#include <world.h>
#include <tweeners.h>

#include <resources.h>

using namespace wok;

Player::Player() :
    texture(res::get<sf::Texture>(textureName)),
    body(*texture, bodyTextureRect),
    gun(*texture, gunTextureRect),
    muzzleFlash(*texture, muzzleFlashTextureRect)
{
    // Bottom-Middle
    sf::Vector2f bodyPivot(
        (float)(bodyTextureRect.width / 2),
        (float)(bodyTextureRect.height)
    );

    gunOffsetInRelationToPivot = gunOffset - bodyPivot;

    body.setOrigin(bodyPivot);
    gun.setOrigin(gunOrigin);
    muzzleFlash.setOrigin(0, muzzleFlash.getTextureRect().height / 2.f); // Middle-Left
}

void Player::flipIfNeeded(sf::Vector2f mousePosition)
{
    bool shouldFaceRight = mousePosition.x > body.getPosition().x;
    if (shouldFaceRight != isFacingRight)
    {
        flip();
    }
}

void Player::flip()
{
    isFacingRight = !isFacingRight;
    float nextScale = isFacingRight ? 1.f : -1.f;

    if (flipTween)
        flipTween->kill();

    flipTween = std::make_shared<LerpTweener<float>>(handle,
        [this]() { return body.getScale().x; }, [this](float v) { body.setScale(v, 1); },
        nextScale, flipTime
        );

    world::addTween(flipTween);
}

std::pair<sf::Vector2f, float> Player::updateGunPositionAndRotation(sf::Vector2f mousePosition)
{
    auto globalGunPos = body.getPosition() + m::scale(gunOffsetInRelationToPivot, body.getScale());
    auto rightDirection = sf::Vector2f(body.getScale().x, 0);

    float angleOfGun = m::angle(rightDirection, mousePosition - globalGunPos);

    gun.setPosition(globalGunPos);
    gun.setRotation(angleOfGun);
    gun.setScale(body.getScale());

    return { globalGunPos, angleOfGun };
}

m::Ray wok::Player::getGunRay()
{
    sf::Vector2f gunDirection = m::rotate(sf::Vector2f(body.getScale().x, 0), gun.getRotation());
    return m::Ray(gun.getPosition(), gunDirection);
}

void Player::shoot(sf::Vector2f globalGunPosition, m::Ray gunRay)
{
    muzzleFlash.setPosition(globalGunPosition + m::rotate(muzzleFlashOffset, m::angle(gunRay.direction)));
    muzzleFlash.setRotation(gun.getRotation());
    muzzleFlash.setScale(body.getScale());

    shouldRenderMuzzleFlash = true;
    muzzleFlash.setColor(sf::Color(0xFFFFFFFF));

    auto muzzleFlashAnimation = std::make_shared<LerpTweener<sf::Color>>(handle,
        [this]() { return muzzleFlash.getColor(); }, [this](auto v) { muzzleFlash.setColor(v); },
        sf::Color(0xFFFFFF00), muzzleFlashTime
        );
    muzzleFlashAnimation->setAfterKilled([this]() { shouldRenderMuzzleFlash = false; });

    world::addTween(muzzleFlashAnimation);

    world::createNamedActor<Bullet>("Bullet",
        muzzleFlash.getPosition(), m::rotate(gunRay.direction, ((rand() / (float)RAND_MAX) - 0.5f) * bulletSpread * 2.f));
}

void Player::updateShootingLogic(sf::Vector2f globalGunPosition, m::Ray gunRay, const GameClock& time)
{
    if (shootCooldown <= 0)
    {
        if (input::attack.isPressed)
        {
            shootCooldown += shootInterval;
            shoot(globalGunPosition, gunRay);
        }
    }
    else
    {
        shootCooldown -= time.delta;
    }
}

void wok::Player::update(const GameClock& time)
{
    auto mousePosition = input::mousePositionInWorld;

    body.move(m::normalize(input::movement) * time.delta * movementSpeed);
    flipIfNeeded(mousePosition);

    auto gunPlacement = updateGunPositionAndRotation(mousePosition);
    auto gunRay = getGunRay();
    updateShootingLogic(gunPlacement.first, gunRay, time);
}

void wok::Player::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    target.draw(gun, states);

    if (shouldRenderMuzzleFlash)
    {
        target.draw(muzzleFlash);
    }
}
