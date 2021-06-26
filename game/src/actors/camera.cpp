#include <actors/camera.h>

#include <gameState.h>

wok::Camera::Camera(std::shared_ptr<CameraSettings> settings) :
    settings(settings)
{ }

void wok::Camera::start(const GameClock&)
{
    game::setActiveCamera(handle.as<Camera>());
}

void wok::Camera::update(const GameClock& time)
{
    if (!followTarget.isValid())
        return;

    //sf::Vector2f pos = followTarget->getActorPosition();
    //position = { std::round(pos.x), std::round(pos.y) };
    position = followTarget->getActorPosition();
}

void wok::Camera::drawGizmos(sf::RenderTarget& target, sf::RenderStates& states)
{
    sf::CircleShape centre(2);
    centre.setFillColor(sf::Color(0x0000FF88));
    centre.setOutlineColor(sf::Color(0x0000FFFF));
    centre.setOutlineThickness(-1.f);

    centre.setOrigin(sf::Vector2f(1.f, 1.f) * centre.getRadius());

    centre.setPosition(position + settings->offset);

    target.draw(centre, states);
}

sf::View wok::Camera::getView(sf::Vector2f screenSize) const
{
    float ratio = screenSize.x / screenSize.y;

    float height = settings->height;
    float width = height * ratio;

    lastView = sf::View(position, sf::Vector2f(width, height));
    return lastView;
    //return sf::View({ std::round(position.x), std::round(position.y) }, screenSize);
}
