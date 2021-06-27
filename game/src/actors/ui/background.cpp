#include <actors/ui/background.h>

void wok::ui::Background::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    sf::RectangleShape shape((sf::Vector2f)target.getSize());
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(target.getView().getCenter());

    shape.setFillColor(fillColor);

    target.draw(shape, states);
}
