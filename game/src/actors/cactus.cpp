#include <actors/cactus.h>
#include <world.h>
#include <resources.h>
#include <actors.h>
#include <tweeners.h>

#include <rng.h>

wok::Cactus::Cactus(std::shared_ptr<CactusPreset> preset) :
    preset(preset)
{
    texture = res::get<sf::Texture>(preset->textureName);
    health = preset->startingHealth;

    setTexture(*texture);
    assetsReloaded();
}

void wok::Cactus::assetsReloaded()
{
    setTextureRect(preset->textureRect);

    setOrigin(
        (float)(preset->textureRect.width / 2),
        (float)(preset->textureRect.height)
    );
}

void wok::Cactus::start(const GameClock&)
{
    addWindTween();
}

void wok::Cactus::update(const GameClock&)
{
    CollisionContext ctx(CollisionContext::SourceType::Enviroment);

    auto overlapped = world::checkForOverlaps(ctx, handle.as<Collideable>(), getPosition() + sf::Vector2f(0.f, -4.f), 2.f);
    auto hittable = overlapped.as<Hittable>();
    if (hittable.isValid())
    {
        auto actor2D = hittable.as<Actor2D>();
        hittable->reactToHit(
            {
                m::normalize(actor2D->getActorPosition() - getPosition()),
                1
            });
        auto hit = createHitTweener(1.f, 0.5f);
    }
}

void wok::Cactus::addWindTween()
{
    windAnimation = std::make_shared<SineTweener<float>>(
        handle,
        [this](float v) { setRotation(v); },
        -preset->animationScale, preset->animationScale, preset->animationScale
        );
    windAnimation->addTimeOffset(randomizer::getBetween(0.f, 20.f));

    world::addTween(windAnimation);
}

void wok::Cactus::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(*this, states);
}

void wok::Cactus::reactToHit(HitData data)
{
    float dir = data.direction.x < 0
        ? -1.f
        : 1.f;

    health -= data.damage;
    bool shouldDie = health <= 0;

    if (shouldDie)
    {
        world::createNamedActor<FracturedSprite>("Cactus Fracture", *this, texture, preset->fractures, dir);
        handle.destroy();
    }
    else
    {
        auto hit = createHitTweener(dir);
    }
}

void wok::Cactus::getHitboxes(const CollisionContext& ctx, const std::function<void(const physics::Hitbox&)> yield)
{
    if (!ctx.shouldHitDestructible)
        return;

    yield(physics::AABB(getGlobalBounds()));
}

auto wok::Cactus::createHitTweener(float dir, float duration) -> std::shared_ptr<LerpTweener<float>>
{
    windAnimation->paused = true;

    auto hit = std::make_shared<LerpTweener<float>>(handle,
        [this]() { return getRotation(); }, [this](float v) { setRotation(v); },
        getRotation() + 2.f * dir * preset->animationScale, duration
        );

    hit->setEasing([](float t) { return std::sin(t * 4 * 3.1415f) * pow(2.7182f, 0.4f * -t); });

    auto anim = windAnimation;
    hit->setAfterKilled([anim]()
        {
            if (anim) anim->paused = false;
        });

    world::addTween(hit);
    return hit;
}
