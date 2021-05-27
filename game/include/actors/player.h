#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

#include <customShapes/line.h>
#include <assets/playerSettings.h>

namespace wok
{
    class Player : public Actor, public wok::Drawable, public Tickable, public Collideable
    {
    public:
        Player(std::shared_ptr<PlayerSettings> settings);
        virtual void update(const GameClock& time) override;
        virtual void assetsReloaded() override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual void drawGizmos(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual float getSortingYPos() override { return body.getPosition().y; }

        void setPosition(sf::Vector2f v) { body.setPosition(v); }
        sf::Vector2f getPosition() { return body.getPosition(); }

        virtual void getHitboxes(const std::function<void(const physics::Hitbox&)> yield) override;

    private:
        void flipIfNeeded(sf::Vector2f mousePosition);
        void flip();
        std::pair<sf::Vector2f, float> updateGunPositionAndRotation(sf::Vector2f mousePosition);
        m::Ray getGunRay();
        void moveActor(sf::Vector2f delta);
        void applyInputToVelocity(sf::Vector2f, float dt);
        void updateShootingLogic(sf::Vector2f globalGunPosition, m::Ray gunRay, const GameClock& time);
        void shoot(sf::Vector2f globalGunPosition, m::Ray gunRay);

    private:
        const std::shared_ptr<PlayerSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;
        sf::Sprite gun;
        sf::Sprite muzzleFlash;

        sf::Vector2f gunOffsetInRelationToPivot;
        std::shared_ptr<LerpTweener<float>> flipTween;

        bool isFacingRight = true;
        bool shouldRenderMuzzleFlash = false;
        float shootCooldown = 0;
        sf::Vector2f velocity;
    };
}
