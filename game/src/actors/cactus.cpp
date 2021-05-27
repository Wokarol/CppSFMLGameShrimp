#include <actors/cactus.h>
#include <world.h>
#include <resources.h>
#include <actors.h>
#include <tweeners.h>

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

void wok::Cactus::start()
{
    addWindTween();
}

void wok::Cactus::addWindTween()
{
    windAnimation = std::make_shared<SineTweener<float>>(
        handle,
        [this](float v) { setRotation(v); },
        -preset->animationScale, preset->animationScale, preset->animationScale
        );
    windAnimation->addTimeOffset((rand() / (float)RAND_MAX) * 20.f);

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
        windAnimation->paused = true;
        auto hit = createHitTweener(dir);

        auto anim = windAnimation;
        hit->setAfterKilled([anim]()
            {
                if (anim) anim->paused = false;
            });
    }
}

void wok::Cactus::getHitboxes(const std::function<void(const physics::Hitbox&)> yield)
{
    yield(physics::AABB(getGlobalBounds()));
}

auto wok::Cactus::createHitTweener(float dir) -> std::shared_ptr<LerpTweener<float>>
{
    auto hit = std::make_shared<LerpTweener<float>>(handle,
        [this]() { return getRotation(); }, [this](float v) { setRotation(v); },
        getRotation() + 2.f * dir * preset->animationScale, 1.f
        );

    hit->setEasing([](float t) { return std::sin(t * 4 * 3.1415f) * pow(2.7182f, 0.4f * -t); });

    world::addTween(hit);
    return hit;
}
