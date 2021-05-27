#include <physics/hitboxes.h>
#include <SFML/Graphics.hpp>

sf::RectangleShape getRect(sf::Vector2f postion, sf::Vector2f size, sf::Color color)
{
    sf::RectangleShape rect(size);
    rect.setPosition(postion);

    rect.setOutlineThickness(-1);
    rect.setFillColor(sf::Color(0));
    rect.setOutlineColor(color);

    return rect;
}

void wok::physics::AABB::draw(sf::RenderTarget& target, sf::RenderStates& states, sf::Color color) const
{
    auto rect = getRect(topLeft, size, color);
    target.draw(rect, states);
}

void wok::physics::OBB::draw(sf::RenderTarget& target, sf::RenderStates& states, sf::Color color) const
{
    auto rect = getRect(position, size, color);
    rect.setOrigin(size / 2.f);
    rect.setRotation(rotation);

    target.draw(rect, states);
}

void wok::physics::Circle::draw(sf::RenderTarget& target, sf::RenderStates& states, sf::Color color) const
{
    sf::CircleShape circle(radius);

    circle.setOutlineThickness(-1);
    circle.setFillColor(sf::Color(0));
    circle.setOutlineColor(color);

    circle.setOrigin(radius, radius);
    circle.setPosition(position);

    target.draw(circle, states);
}