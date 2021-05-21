#pragma once

#include <actor.h>
#include <memory>
#include <tweeners.h>

#include <customShapes/line.h>

namespace wok
{
    class Player : public Actor, public wok::Drawable, public Tickable
    {
        const float shootInterval = 0.2f;
        const float bulletSpread = 4.f;
        const float muzzleFlashTime = 0.05f;

        const float flipTime = 0.1f;

        const float movementSpeed = 50.f;

        const std::string textureName = "actors/shrimp";
        const sf::IntRect bodyTextureRect = { 0, 0, 13, 14 };
        const sf::IntRect gunTextureRect = { 0, 17, 6, 4 };
        const sf::IntRect muzzleFlashTextureRect = { 7, 15, 7, 8 };

        const sf::Vector2f gunOffset = { 8, 10 };
        const sf::Vector2f gunOrigin = { 1, 1 };
        const sf::Vector2f muzzleFlashOffset = { 5, 0 }; // In relation to a gun

    public:
        Player();

        void setPosition(float x, float y);
        void setPosition(sf::Vector2f v);
        virtual void update(const GameClock& time) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;

        virtual float getSortingYPos() override { return body.getPosition().y; }

    private:
        std::shared_ptr<sf::Texture> texture;

        sf::Sprite body;
        sf::Sprite gun;
        sf::Sprite muzzleFlash;

        sf::Vector2f gunOffsetInRelationToPivot;
        std::shared_ptr<LerpTweener<float>> flipTween;

        bool facingRight = true;
        bool shouldRenderMuzzleFlash = false;
        float shootCooldown = 0;
    };
}
