#include <actors/player.h>
#include <actors/bullet.h>
#include <input.h>
#include <utils/mathUtils.h>
#include <world.h>
#include <tweeners.h>

#include <resources.h>

using namespace wok;

Player::Player(std::shared_ptr<PlayerSettings> settings) :
    settings(settings)
{
    texture = res::get<sf::Texture>(settings->textureName);
    assetsReloaded();
}

void wok::Player::update(const GameClock& time)
{
    auto mousePosition = input::mousePositionInWorld;
    auto inputDir = m::normalize(input::movement);

    applyInputToVelocity(inputDir, time.delta);

    moveActor(velocity * time.delta);

    flipIfNeeded(mousePosition);

    auto gunPlacement = updateGunPositionAndRotation(mousePosition);
    auto gunRay = getGunRay();

    updateShootingLogic(gunPlacement.first, gunRay, time);

    if (input::knockback.wasPressedThisFrame)
    {
        velocity -= gunRay.direction * 200.f;
    }
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

void wok::Player::drawGizmos(sf::RenderTarget& target, sf::RenderStates& states)
{
    auto rect = body.getGlobalBounds();

    sf::RectangleShape collider({ rect.width, rect.height });
    collider.setPosition(rect.left, rect.top);

    collider.setFillColor(sf::Color(0));
    collider.setOutlineColor(sf::Color::Blue);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        collider.setOutlineColor(collider.getOutlineColor() * sf::Color(255, 255, 255, 128));

    collider.setOutlineThickness(-1);

    target.draw(collider, states);
}


// =========== PRIVATE ==========
void wok::Player::assetsReloaded()
{
    body.setTexture(*texture);
    body.setTextureRect(settings->bodyTextureRect);

    gun = sf::Sprite(*texture, settings->gunTextureRect);
    muzzleFlash = sf::Sprite(*texture, settings->muzzleFlashTextureRect);

    // Bottom-Middle
    sf::Vector2f bodyPivot(
        (float)(settings->bodyTextureRect.width / 2),
        (float)(settings->bodyTextureRect.height)
    );

    gunOffsetInRelationToPivot = settings->gunOffset - bodyPivot;

    body.setOrigin(bodyPivot);
    gun.setOrigin(settings->gunOrigin);
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
        nextScale, settings->flipTime
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
    velocity -= gunRay.direction * 50.f;

    muzzleFlash.setPosition(globalGunPosition + m::rotate(settings->muzzleFlashOffset, m::angle(gunRay.direction)));
    muzzleFlash.setRotation(gun.getRotation());
    muzzleFlash.setScale(body.getScale());

    shouldRenderMuzzleFlash = true;
    muzzleFlash.setColor(sf::Color(0xFFFFFFFF));

    auto muzzleFlashAnimation = std::make_shared<LerpTweener<sf::Color>>(handle,
        [this]() { return muzzleFlash.getColor(); }, [this](auto v) { muzzleFlash.setColor(v); },
        sf::Color(0xFFFFFF00), settings->muzzleFlashTime
        );
    muzzleFlashAnimation->setAfterKilled([this]() { shouldRenderMuzzleFlash = false; });

    world::addTween(muzzleFlashAnimation);

    sf::Vector2f position = muzzleFlash.getPosition();
    sf::Vector2f direction = m::rotate(gunRay.direction, ((rand() / (float)RAND_MAX) - 0.5f) * settings->bulletSpread * 2.f);

    world::createNamedActor<Bullet>("Bullet", position, direction, res::get<BulletSettings>(settings->bulletName));
}

void Player::updateShootingLogic(sf::Vector2f globalGunPosition, m::Ray gunRay, const GameClock& time)
{
    if (shootCooldown <= 0)
    {
        if (input::attack.isPressed)
        {
            shootCooldown += settings->shootInterval;
            shoot(globalGunPosition, gunRay);
        }
    }
    else
    {
        shootCooldown -= time.delta;
    }
}

void wok::Player::moveActor(sf::Vector2f delta)
{
    body.move(delta);

    // Rection to world geometry
    std::vector<collide::Reaction> reactions;
    world::checkForCollisions(body.getGlobalBounds(), reactions);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return;

    sf::Vector2f accumulatedReaction;
    for (const auto& r : reactions)
    {
        auto p = -r.penetration;
        if (p.x != 0 && p.y != 0)
        {
            if (std::abs(p.x) < std::abs(p.y))
                p.y = 0;
            else if (std::abs(p.x) > std::abs(p.y))
                p.x = 0;
        }

        if (std::abs(p.x) > std::abs(accumulatedReaction.x))
            accumulatedReaction.x = p.x;
        if (std::abs(p.y) > std::abs(accumulatedReaction.y))
            accumulatedReaction.y = p.y;
    }
    body.move(accumulatedReaction);
}

void Player::applyInputToVelocity(sf::Vector2f input, float dt)
{
    sf::Vector2f desiredVelocity = input * settings->movementSpeed;

    if (std::abs(desiredVelocity.x) < std::abs(velocity.x))
    {
        float signX = m::sign(velocity.x);
        velocity.x -= signX * 300.f * dt;
    }
    else
    {
        velocity.x = m::lerp(velocity.x, desiredVelocity.x, 0.7f);
    }

    if (std::abs(desiredVelocity.y) < std::abs(velocity.y))
    {
        float signY = m::sign(velocity.y);
        velocity.y -= signY * 300.f * dt;
    }
    else
    {
        velocity.y = m::lerp(velocity.y, desiredVelocity.y, 0.7f);
    }
}