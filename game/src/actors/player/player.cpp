#include <actors/player.h>
#include <actors/bullet.h>
#include <actors/fracturedSprite.h>

#include <input.h>
#include <utils/mathUtils.h>
#include <world.h>
#include <tweeners.h>

#include <resources.h>

using namespace wok;

Player::Player(std::shared_ptr<PlayerSettings> settings) :
    settings(settings)
{
    health = settings->maxHealth;
    texture = res::get<sf::Texture>(settings->textureName);
}

void wok::Player::start()
{
    movement = Movement2D(handle, settings->movement, [this]() { return body.getGlobalBounds(); });
    gun = Gun(handle, settings->gun, texture);

    healthBar = world::createNamedActor<IconBar>("Player Health",
        res::get<IconBarSettings>(settings->healthBarName), settings->maxHealth);

    assetsReloaded();
}

void wok::Player::update(const GameClock& time)
{
    auto mousePosition = input::mousePositionInWorld;
    auto inputDir = m::normalize(input::movement);

    movement.moveBy(body, inputDir, time.delta);
    movement.setOrientation(body, mousePosition);

    gun.update(body, mousePosition, time);

    if (invincibilityCooldown > 0.f) invincibilityCooldown -= time.delta;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F8))
    {
        health = settings->maxHealth;
        if (healthBar.isValid()) healthBar->setValue(health);
    }
}

void wok::Player::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    gun.draw(target, states);
}

void wok::Player::drawGizmos(sf::RenderTarget& target, sf::RenderStates& states)
{
    auto rect = body.getGlobalBounds();

    sf::RectangleShape collider({ rect.width, rect.height });
    collider.setPosition(rect.left, rect.top);

    collider.setFillColor(sf::Color(0x3094ff55));

    target.draw(collider, states);
}

void wok::Player::getHitboxes(const std::function<void(const physics::Hitbox&)> yield)
{
    yield(physics::AABB(body.getGlobalBounds()));
}

void wok::Player::reactToHit(HitData data)
{
    if (invincibilityCooldown >= 0.f)
        return;

    movement.applyPushback(data.direction * 165.f);
    invincibilityCooldown = invincibilityLength;

    health -= data.damage;
    if (healthBar.isValid()) healthBar->setValue(health);

    if (health <= 0)
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

        world::createNamedActor<FracturedSprite>("Player Fracture", body, texture, fractures, dir);
        handle.destroy();
    }
}

// =========== PRIVATE ==========
void wok::Player::assetsReloaded()
{
    body.setTexture(*texture);
    body.setTextureRect(settings->bodyTextureRect);

    // Bottom-Middle
    sf::Vector2f bodyPivot(
        (float)(settings->bodyTextureRect.width / 2),
        (float)(settings->bodyTextureRect.height)
    );

    body.setOrigin(bodyPivot);
    gun.loadAssets(bodyPivot);
}
