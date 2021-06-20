#include <actors/basicEnemy.h>
#include <world.h>

#include <resources.h>
#include <console.h>
#include <gameState.h>

#include <rng.h>
#include <utils/mathUtils.h>

#include <actors/fracturedSprite.h>

wok::BasicEnemy::BasicEnemy(std::shared_ptr<EnemySettings> settings) :
    settings(settings)
{
    texture = res::get<sf::Texture>(settings->textureName);
}

void wok::BasicEnemy::start(const GameClock& time)
{
    movement = Movement2D(handle, settings->movement, [this]() { return body.getGlobalBounds(); });
    gun = Gun(handle, settings->gun, texture);
    health = Health(handle, settings->maxHealth, [this](HitData d) { onDeath(d); });

    randomStartTimeForShooting = time.absolute + randomizer::getBetween(0.f, 1.f);

    assetsReloaded();
}

void wok::BasicEnemy::assetsReloaded()
{
    body.setTexture(*texture);
    body.setTextureRect(settings->textureRect);

    // Bottom-Middle
    sf::Vector2f bodyPivot(
        (float)(settings->textureRect.width / 2),
        (float)(settings->textureRect.height)
    );

    body.setOrigin(bodyPivot);
    gun.loadAssets(bodyPivot);
}

void wok::BasicEnemy::update(const GameClock& time)
{
    if (time.absolute < randomStartTimeForShooting)
        return;

    if (directionChangedGate.executeAfter(.5f, time.delta))
    {
        chosenDirection = m::rotate(sf::Vector2f(1.f, 0.f), randomizer::getBetween(0.f, 360.f));
    }

    sf::Vector2f aimTarget = game::player.isValid()
        ? game::player->getActorPosition() + sf::Vector2f(0.f, -10.f)
        : gun.getPosition() + sf::Vector2f(m::sign(chosenDirection.x) * 100.f, 0.f);

    movement.moveBy(body, chosenDirection, time.delta);
    movement.setOrientation(body, aimTarget);

    gun.update(body.getPosition(), body.getScale(), aimTarget, time, game::player.isValid());
}

void wok::BasicEnemy::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    gun.draw(target, states);
}

void wok::BasicEnemy::getHitboxes(const std::function<void(const physics::Hitbox&)> yield)
{
    yield(physics::AABB(body.getGlobalBounds()));
}

void wok::BasicEnemy::reactToHit(HitData data)
{
    movement.applyPushback(data.direction * 30.f);
    health.damage(data);
}

void wok::BasicEnemy::onDeath(HitData data)
{
    float dir = data.direction.x < 0
        ? -1.f
        : 1.f;

    std::vector<sf::IntRect> fractures{
        {3, 0, 7, 4},
        {0, 0, 3, 4},
        {10, 2, 2, 2},
        {4, 4, 8, 5},
        {0, 5, 4, 5},
        {1, 10, 4, 4},
    };

    world::createNamedActor<FracturedSprite>("Enemy Fracture", body, texture, fractures, dir);
    handle.destroy();
}


