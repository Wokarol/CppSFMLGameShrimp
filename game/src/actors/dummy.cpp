#include <actors\dummy.h>

#include <actors\fracturedSprite.h>

#include <gameState.h>
#include <resources.h>

wok::Dummy::Dummy(std::shared_ptr<DummySettings> settings, std::string message) :
    settings(settings), message(message)
{
    game::dummyKilled = false;

    texture = res::get<sf::Texture>(settings->textureName);
    font = res::get<sf::Font>("RobotoMono");
    body.setTexture(*texture);
    assetsReloaded();

    float scaleFactor = 2.f;

    messageText = sf::Text(message, *font, 8U * scaleFactor);
    messageText.scale(1.f / scaleFactor, 1.f / scaleFactor);

    messageText.setColor(sf::Color::White);
    messageText.setOutlineColor(sf::Color(128, 128, 128));
    messageText.setOutlineThickness(2);

    auto messageTextLocalBounds = messageText.getLocalBounds();
    sf::Vector2f messageTextSize(messageTextLocalBounds.width, messageTextLocalBounds.height);
    sf::Vector2f messageTextOffset(messageTextLocalBounds.left, messageTextLocalBounds.top);
    messageText.setOrigin(m::scale(messageTextSize, sf::Vector2f(0.5f, 1.f)) + messageTextOffset);
}

void wok::Dummy::assetsReloaded()
{
    body.setTextureRect(settings->textureRect);

    body.setOrigin(
        (float)(settings->textureRect.width / 2),
        (float)(settings->textureRect.height)
    );
}

void wok::Dummy::update(const GameClock& time)
{
    float t = time.absolute * 5.f;

    sf::Vector2f offset(0.f, m::lerp(-2.f, 2.f, std::sin(t)));
    messageText.setPosition(body.getPosition() + sf::Vector2f(0, -16) + offset);

    sf::Vector2f scale(m::lerp(1.f, 1.15f, std::sin(t)), m::lerp(1.f, 0.90f, std::sin(t)));
    body.setScale(scale);
}

void wok::Dummy::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(body, states);
    target.draw(messageText);
}

void wok::Dummy::reactToHit(HitData data)
{
    float dir = data.direction.x < 0
        ? -1.f
        : 1.f;

    world::createNamedActor<FracturedSprite>("Dummy Fracture", body, texture, settings->fractures, dir);
    handle.destroy();

    game::dummyKilled = true;
}

void wok::Dummy::getHitboxes(const CollisionContext& ctx, const std::function<void(const physics::Hitbox&)> yield)
{
    if (!ctx.shouldHitDestructible)
        return;

    yield(physics::AABB(body.getGlobalBounds()));
}
