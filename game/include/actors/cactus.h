#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <tweeners.h>
#include <assets/cactusPreset.h>

namespace wok
{
    class Cactus : public sf::Sprite,
        public Actor, public wok::Drawable, public Hittable
    {
        const std::shared_ptr<CactusPreset> preset;

    public:
        Cactus(std::shared_ptr<CactusPreset> preset);

        virtual void assetsReloaded() override;
        virtual void start() override;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;;
        virtual float getSortingYPos() override { return getPosition().y; }

        virtual intersect::Intersection getClosestHit(const m::Ray& ray) override;
        virtual void reactToHit(const intersect::Intersection& intersection, int damage) override;

    private:
        void addWindTween();
        std::shared_ptr<LerpTweener<float>> createHitTweener(float dir);

        int health;
        bool dying = false;
        std::shared_ptr<sf::Texture> texture;
        std::shared_ptr<SineTweener<float>> windAnimation;
    };
}
