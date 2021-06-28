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
    auto sourceType = CollisionContext::SourceType::Enemy;

    movement = Movement2D(handle, sourceType, settings->movement, [this]() { return body.getGlobalBounds(); });
    gun = Gun(handle, sourceType, settings->gun, texture);
    health = Health(handle, settings->maxHealth, [this](HitData d) { onDeath(d); });

    randomStartTimeForShooting = time.absolute + randomizer::getBetween(0.f, 1.f);

    assetsReloaded();

    gun.update(body.getPosition(), body.getScale(), sf::Vector2f(1.f, 0.f), time);
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

    sf::Vector2f playerPosition = game::player.isValid()
        ? game::player->getActorPosition() + sf::Vector2f(0.f, -10.f)
        : gun.getPosition() + sf::Vector2f(m::sign(chosenDirection.x) * 100.f, 0.f);

    movement.moveBy(body, chosenDirection, time.delta);
    movement.setOrientation(body, playerPosition);

    sf::Vector2f gunPos = gun.getPosition();
    bool canSeeThePlayer = false;
    if (game::player.isValid())
    {
        m::Ray playerRay(gunPos, playerPosition - gunPos);

        CollisionContext ctx(CollisionContext::SourceType::Enemy);
        ctx.shouldHitTransparent = false;
        ctx.shouldHitDestructible = false;
        auto rayResult = world::raycastAgainstHitboxes(ctx, playerRay, m::dist(gunPos, playerPosition));

        if (!rayResult.intersection.hit)
        {
            // We did not hit something on our way to player so we can shoot 'em
            canSeeThePlayer = true;
        }
    }

    sf::Vector2f currentAimTarget = canSeeThePlayer
        ? playerPosition
        : gunPos + (sf::Vector2f(m::sign(body.getScale().x), 0) * 1000.f);

    gun.update(body.getPosition(), body.getScale(), currentAimTarget, time);

    if (canSeeThePlayer)
    {
        gun.tryToShoot();
    }
}

void wok::BasicEnemy::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    gun.draw(target, states);
}

void wok::BasicEnemy::getHitboxes(const CollisionContext& ctx, const std::function<void(const physics::Hitbox&)> yield)
{
    if (!ctx.shouldHitTransparent)
        return;

    if (ctx.sourceType == CollisionContext::SourceType::Enemy)
        return;

    yield(physics::AABB(body.getGlobalBounds()));
}

void wok::BasicEnemy::reactToHit(HitData data)
{
    movement.applyPushback(data.direction * 60.f);
    health.damage(data);
}

void wok::BasicEnemy::onDeath(HitData data)
{
    float dir = data.direction.x < 0
        ? -1.f
        : 1.f;

    world::createNamedActor<FracturedSprite>("Enemy Fracture", body, texture, settings->fractures, dir);
    handle.destroy();

    game::score.addPoints(1);
}


