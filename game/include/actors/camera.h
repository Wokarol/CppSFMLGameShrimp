#pragma once

#include <actor.h>
#include <world.h>
#include <assets/cameraSettings.h>

#include <viewportCamera.h>

namespace wok
{
    class Camera : public Actor2D, public Tickable, public ViewportCamera
    {
    public:
        Camera(std::shared_ptr<CameraSettings> settings);

        virtual void start(const GameClock&) override;
        virtual void update(const GameClock& time) override;
        virtual void drawGizmos(sf::RenderTarget&, sf::RenderStates&) override;

        virtual sf::View getView(sf::Vector2f screenSize) const override;

        // Getters / Setters
        virtual void setFollowTarget(ActorHandle<Actor2D> target) { followTarget = target; }
        virtual sf::Vector2f getActorPosition() { return position; }

    private:
        std::shared_ptr<CameraSettings> settings;
        ActorHandle<Actor2D> followTarget;

        sf::Vector2f position;
    };
}