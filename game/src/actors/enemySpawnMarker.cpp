#include <actors/enemySpawnMarker.h>

wok::EnemySpawnMarker::EnemySpawnMarker(float spawnDuration, sf::Vector2f position, std::function<void()> spawnCallback) :
    spawnDuration(spawnDuration), position(position), spawnCallback(spawnCallback)
{
    timeLeftUntilSpawn = spawnDuration;
}

void wok::EnemySpawnMarker::update(const GameClock& time)
{
    timeLeftUntilSpawn -= time.delta;

    if (timeLeftUntilSpawn < 0)
    {
        spawnCallback();
        handle.destroy();
    }
}

void wok::EnemySpawnMarker::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    float t = timeLeftUntilSpawn / spawnDuration;

    sf::RectangleShape marker(sf::Vector2f(1.f, 1.f) * m::lerp(0.f, 20.f, t));
    marker.setFillColor(sf::Color(0xFF005588));
    marker.setOrigin(marker.getSize() / 2.f);

    marker.setPosition(position);
    marker.setRotation(m::lerp(0.f, 180.f, t));

    target.draw(marker, states);
}
