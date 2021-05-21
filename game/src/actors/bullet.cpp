#include <actors/bullet.h>
#include <resources.h>
#include <utils/mathUtils.h>
#include <world.h>

wok::Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction, BulletSettings settings) :
    settings(settings),
    direction(m::normalize(direction)), texture(res::get<sf::Texture>(settings.textureName))
{
    setPosition(position);
    setRotation(m::angle(direction));

    setTexture(*texture);
    setTextureRect(settings.textureRect);
    setOrigin(getTextureRect().width / 2.f, getTextureRect().width / 2.f); // Centre
}

void wok::Bullet::update(const GameClock& time)
{
    timeSinceStart += time.delta;
    if (timeSinceStart > settings.lifespan)
    {
        handle.destroy();
        return;
    }

    float distanceDelta = settings.velocity * time.delta;

    auto raycastResult = world::raycast(m::Ray(getPosition(), direction), distanceDelta);
    if (raycastResult.hitActor.isValid())
    {
        raycastResult.hitActor->reactToHit(raycastResult.intersection, settings.damage);
        handle.destroy();
    }

    move(direction * distanceDelta);
}

void wok::Bullet::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(*this, states);
}
