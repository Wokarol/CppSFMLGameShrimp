#include <actors/basicEnemy.h>
#include <world.h>

#include <resources.h>

wok::BasicEnemy::BasicEnemy(std::shared_ptr<EnemySettings> settings) :
    settings(settings)
{
    texture = res::get<sf::Texture>(settings->textureName);
}

void wok::BasicEnemy::start()
{
    movement = Movement2D(handle, settings->movement, [this]() { return body.getGlobalBounds(); });
    gun = Gun(handle, settings->gun, texture);

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
    movement.moveBy(body, sf::Vector2f(-1.f, 0.f), time.delta);
    movement.setOrientation(body, false);

    gun.update(body.getPosition(), body.getScale(), gun.getPosition() + sf::Vector2f(-50.f, 0.f), time, false);
}

void wok::BasicEnemy::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    gun.draw(target, states);
}


