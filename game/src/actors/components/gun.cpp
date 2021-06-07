#include <actors/components/gun.h>
#include <utils/mathUtils.h>
#include <tweeners.h>
#include <world.h>
#include <resources.h>

#include <actors/bullet.h>

void wok::Gun::update(const sf::Transformable& body, sf::Vector2f aimTarget, GameClock time)
{
    auto gunPlacement = updateGunPositionAndRotation(body, aimTarget);
    auto gunRay = getGunRay(body);

    updateShootingLogic(body, gunPlacement.first, gunRay, time);
}

void wok::Gun::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(gun, states);

    if (shouldRenderMuzzleFlash)
    {
        target.draw(muzzleFlash);
    }
}

void wok::Gun::loadAssets(sf::Vector2f bodyPivot)
{
    gun = sf::Sprite(*texture, settings->gunTextureRect);
    muzzleFlash = sf::Sprite(*texture, settings->muzzleFlashTextureRect);

    gunOffsetInRelationToPivot = settings->gunOffset - bodyPivot;

    gun.setOrigin(settings->gunOrigin);
    muzzleFlash.setOrigin(0, muzzleFlash.getTextureRect().height / 2.f); // Middle-Left
}

auto wok::Gun::updateGunPositionAndRotation(const sf::Transformable& body, sf::Vector2f aimTarget) -> std::pair<sf::Vector2f, float>
{
    auto globalGunPos = body.getPosition() + m::scale(gunOffsetInRelationToPivot, body.getScale());
    auto rightDirection = sf::Vector2f(body.getScale().x, 0);

    float angleOfGun = m::angle(rightDirection, aimTarget - globalGunPos);

    gun.setPosition(globalGunPos);
    gun.setRotation(angleOfGun);
    gun.setScale(body.getScale());

    return { globalGunPos, angleOfGun };
}

auto wok::Gun::getGunRay(const sf::Transformable& body) -> m::Ray
{
    sf::Vector2f gunDirection = m::rotate(sf::Vector2f(body.getScale().x, 0), gun.getRotation());
    return m::Ray(gun.getPosition(), gunDirection);
}

void wok::Gun::updateShootingLogic(const sf::Transformable& body, sf::Vector2f globalGunPosition, m::Ray gunRay, const GameClock& time)
{
    if (shootCooldown <= 0)
    {
        if (input::attack.isPressed)
        {
            shootCooldown += settings->shootInterval;
            shoot(body, globalGunPosition, gunRay);
        }
    }
    else
    {
        shootCooldown -= time.delta;
    }
}

void wok::Gun::shoot(const sf::Transformable& body, sf::Vector2f globalGunPosition, m::Ray gunRay)
{
    muzzleFlash.setPosition(globalGunPosition + m::rotate(settings->muzzleFlashOffset, m::angle(gunRay.direction)));
    muzzleFlash.setRotation(gun.getRotation());
    muzzleFlash.setScale(body.getScale());

    shouldRenderMuzzleFlash = true;
    muzzleFlash.setColor(sf::Color(0xFFFFFFFF));

    auto muzzleFlashAnimation = std::make_shared<LerpTweener<sf::Color>>(owner,
        [this]() { return muzzleFlash.getColor(); }, [this](auto v) { muzzleFlash.setColor(v); },
        sf::Color(0xFFFFFF00), settings->muzzleFlashTime
        );
    muzzleFlashAnimation->setAfterKilled([this]() { shouldRenderMuzzleFlash = false; });

    world::addTween(muzzleFlashAnimation);

    sf::Vector2f position = muzzleFlash.getPosition();
    sf::Vector2f direction = m::rotate(gunRay.direction, ((rand() / (float)RAND_MAX) - 0.5f) * settings->bulletSpread * 2.f);

    world::createNamedActor<Bullet>("Bullet", position, direction, res::get<BulletSettings>(settings->bulletPath));
}
