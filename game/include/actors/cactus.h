#pragma once

#include <actor.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <tweeners.h>
#include <assets/cactusPreset.h>

namespace wok
{
    class Cactus : public sf::Sprite,
        public Actor2D, public wok::Drawable, public Hittable, public Collideable, public Tickable
    {
        const std::shared_ptr<CactusPreset> preset;

    public:
        Cactus(std::shared_ptr<CactusPreset> preset);

        virtual void assetsReloaded() override;
        virtual void start() override;
        virtual void update(const GameClock& time) override;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual float getSortingYPos() override { return getPosition().y; }


        virtual void reactToHit(HitData) override;
        virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield) override;

        virtual sf::Vector2f getActorPosition() override { return getPosition(); }

    private:
        void addWindTween();
        std::shared_ptr<LerpTweener<float>> createHitTweener(float dir, float duration = 1.f);

        int health;
        bool dying = false;
        std::shared_ptr<sf::Texture> texture;
        std::shared_ptr<SineTweener<float>> windAnimation;
    };
}
