#include <actors/cactus.h>
#include <world.h>
#include <resources.h>
#include <actors.h>

wok::Cactus::Cactus(CactusPreset preset) :
    preset(preset)
{
    texture = res::get<sf::Texture>(preset.textureName);
    health = preset.startingHealth;

    setTexture(*texture);
    setTextureRect(preset.textureRect);

    setOrigin(
        (float)(preset.textureRect.width / 2),
        (float)(preset.textureRect.height)
    );
}

void wok::Cactus::start()
{
    animation = std::make_shared<SineTweener<float>>(
        handle,
        [this](float v) { setRotation(v); },
        -preset.animationScale, preset.animationScale, preset.animationScale
        );
    animation->addTimeOffset((rand() / (float)RAND_MAX) * 20.f);

    world::addTween(animation);
}

void wok::Cactus::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(*this, states);
}

wok::intersect::Intersection wok::Cactus::getClosestHit(const m::Ray& ray)
{
    if (dying)
    {
        return {};
    }

    auto bounds = getGlobalBounds();
    sf::RectangleShape collider({ bounds.width, bounds.height });
    collider.setPosition(bounds.left, bounds.top);

    return intersect::rayWithAABB(ray, collider);
}

void wok::Cactus::reactToHit(const intersect::Intersection& intersection, int damage)
{
    float dir = 1;
    if (intersection.ray.direction.x < 0)
    {
        dir = -1;
    }

    health -= damage;
    bool shouldDie = health <= 0;

    if (shouldDie)
    {
        world::createNamedActor<FracturedSprite>("Cactus Fracture", *this, texture, preset.fractures, dir);
        handle.destroy();
    }
    else
    {
        animation->paused = true;
        auto hit = std::make_shared<LerpTweener<float>>(handle,
            [this]() { return getRotation(); }, [this](float v) { setRotation(v); },
            getRotation() + 2.f * dir * preset.animationScale, 1.f
            );

        hit->setEasing([](float t) { return std::sin(t * 4 * 3.1415f) * pow(2.7182f, 0.4f * -t); });

        auto h = handle;
        auto anim = animation;
        hit->setAfterKilled([anim, shouldDie, h]()
            {
                if (anim) anim->paused = false;
            });

        world::addTween(hit);
    }
}
