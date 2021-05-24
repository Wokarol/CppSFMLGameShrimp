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

void wok::Player::update(const GameClock& time)
{
    auto mousePosition = input::mousePositionInWorld;

    body.move(m::normalize(input::movement) * time.delta * settings->movementSpeed);

    std::vector<collide::Reaction> reactions;
    world::collide(body.getGlobalBounds(), reactions);

    for (auto& r : reactions)
    {
        body.move(-r.penetration);
    }

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

void wok::Player::drawGizmos(sf::RenderTarget& target, sf::RenderStates& states)
{
    auto rect = body.getGlobalBounds();

    sf::RectangleShape collider({ rect.width, rect.height });
    collider.setPosition(rect.left, rect.top);

    collider.setFillColor(sf::Color(0));
    collider.setOutlineColor(sf::Color::Blue);
    collider.setOutlineThickness(-1);

    target.draw(collider, states);
}
